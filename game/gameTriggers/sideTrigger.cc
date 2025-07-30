#include "sideTrigger.h"
#include "../../game/cell.h"
#include "../../game/game_state.h"
#include "../../game/link.h"
#include "../../game/player.h"
#include "../../utils/event_types.h"
#include "../../utils/message_queue.h"
#include "../../utils/payload.h"
#include <memory>
#include <stdexcept>

using namespace std;

SideTrigger::SideTrigger(GameState &gameState, const Position &pos)
    : Trigger{gameState, pos, Permission{}} {
  // Create a trigger action that will be executed when the trigger is triggered
  triggerAction = [this]() {
    // Get the cell at the trigger position
    Cell &cell = this->gameState.getBoard().getCell(this->position);
    shared_ptr<Link> link = nullptr;
    // Find the link in the cell
    for (auto &occupant : cell.getOccupants()) {
      link = dynamic_pointer_cast<Link>(occupant);
      if (link)
        break;
    }
    // If the link is found
    if (link) {
      // Get the new position for the link
      Position newPos = this->position;
      int boardSize = this->gameState.getBoard().getGridSize();
      // If the link is on the top edge, set the new position to the bottom edge
      if (this->position.getPosition().second == 0) {
        newPos.setPosition(this->position.getPosition().first, boardSize);
      } else { // If the link is on the bottom edge, set the new position to the
               // top edge
        newPos.setPosition(this->position.getPosition().first, 1);
      }

      shared_ptr<Player> owner = link->permission.getOwner();
      // If the new position is valid, move the link
      if (this->gameState.getBoard().isValidPosition(
              newPos, owner->getPlayerNumber())) {
        this->gameState.removeOccupant(link, this->position);
        this->gameState.addOccupant(link, newPos);
        notifySideTriggered(link->getName(), newPos);
      } else { // If the new position is invalid, throw an error
        if (this->position.getPosition().second == 0) {
          newPos.setPosition(this->position.getPosition().first, 1);
        } else {
          newPos.setPosition(this->position.getPosition().first, boardSize);
        }
        // Move the link to the new position
        this->gameState.removeOccupant(link, this->position);
        this->gameState.addOccupant(link, newPos);
        notifySideTriggered(link->getName(), newPos);
        throw invalid_argument("Invalid move");
      }
    }
  };
}

void SideTrigger::notifySideTriggered(char link, Position &newPos) {
  auto queue = MessageQueue::getInstance();
  map<string, string> payloadMap = {
      {"occupant", string(1, link)},
      {"x", to_string(newPos.getPosition().first)},
      {"y", to_string(newPos.getPosition().second)}};

  Payload payload{payloadMap};
  EventType eventType = EventType::OccupantAdded;
  queue->enqueueEvent(GameEvent(eventType, payload));
}
