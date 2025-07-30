#include "battleTrigger.h"
#include "../../game/cell.h"
#include "../../game/game_state.h"
#include "../../game/player.h"
#include "../../utils/event_types.h"
#include "../../utils/message_queue.h"
#include "../../utils/payload.h"
#include "../../utils/permission.h"
#include <memory>

using namespace std;

BattleTrigger::BattleTrigger(GameState &gameState, const Position &pos)
    : Trigger{gameState, pos, Permission{}} {
  // Create a trigger action that will be executed when the trigger is triggered
  triggerAction = [this]() {
    // Get the cell at the trigger position
    Cell &cell = this->gameState.getBoard().getCell(this->position);
    shared_ptr<Link> attacker = nullptr;
    shared_ptr<Link> defender = nullptr;
    // Find the attacker and defender links in the cell
    for (auto &occupant : cell.getOccupants()) {
      if (dynamic_pointer_cast<Link>(occupant)) {
        shared_ptr<Link> link = dynamic_pointer_cast<Link>(occupant);
        // If the link is a defender, set it as the defender
        if (defender) {
          attacker = link;
        } else {
          // If the link is not a defender, set it as the attacker
          defender = link;
        }
      }
    }
    // If both attacker and defender are found
    if (attacker && defender) {
      // Get the owners of the attacker and defender
      shared_ptr<Player> attackerOwner = attacker->permission.getOwner();
      shared_ptr<Player> defenderOwner = defender->permission.getOwner();
      // Add the defender's owner to the attacker's visibility list
      attacker->permission.addViewer(defenderOwner);
      defender->permission.addViewer(attackerOwner);

      if (attacker->getStrength() >= defender->getStrength()) {
        this->gameState.downloadLink(defender, attackerOwner);
        notifyBattleTriggered(attacker->getName(), this->position);
      } else {
        this->gameState.downloadLink(attacker, defenderOwner);
        notifyBattleTriggered(defender->getName(), this->position);
      }
    }
    if (this->gameState.isWon()) {
      this->gameState.endGame();
    }
  };
}

void BattleTrigger::notifyBattleTriggered(char winner, Position &position) {
  auto queue = MessageQueue::getInstance();
  map<string, string> payloadMap = {
      {"winner", string(1, winner)},
      {"x", to_string(position.getPosition().first)},
      {"y", to_string(position.getPosition().second)}};
  Payload payload{payloadMap};
  EventType eventType = EventType::BattleTriggered;
  queue->enqueueEvent(GameEvent(eventType, payload));
}
