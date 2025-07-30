#include "battleTrigger.h"

#include <iostream>
#include <memory>

#include "../../controller/event_types.h"
#include "../../game/cell.h"
#include "../../game/game_state.h"
#include "../../game/player.h"
#include "../../utils/message_queue.h"
#include "../../utils/payload.h"
#include "../../utils/permission.h"

using namespace std;

BattleTrigger::BattleTrigger(GameState &gameState, const Position &pos)
    : Trigger{gameState, pos, Permission{}} {
  triggerAction = [this]() {
    Cell &cell = this->gameState.getBoard().getCell(this->position);
    shared_ptr<Link> attacker = nullptr;
    shared_ptr<Link> defender = nullptr;
    for (auto &occupant : cell.getOccupants()) {
      if (dynamic_pointer_cast<Link>(occupant)) {
        shared_ptr<Link> link = dynamic_pointer_cast<Link>(occupant);
        if (defender) {
          attacker = link;
        } else {
          defender = link;
        }
      }
    }
    if (attacker && defender) {
      shared_ptr<Player> attackerOwner = attacker->permission.getOwner();
      shared_ptr<Player> defenderOwner = defender->permission.getOwner();
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

BattleTrigger::~BattleTrigger() {}