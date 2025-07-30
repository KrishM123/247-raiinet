#include "sideTrigger.h"

#include <iostream>
#include <memory>

#include "../../controller/event_types.h"
#include "../../game/cell.h"
#include "../../game/game_state.h"
#include "../../game/link.h"
#include "../../game/player.h"
#include "../../utils/message_queue.h"
#include "../../utils/payload.h"

using namespace std;

SideTrigger::SideTrigger(GameState &gameState, const Position &pos)
    : Trigger{gameState, pos, Permission{}} {
  triggerAction = [this]() {
    Cell &cell = this->gameState.getBoard().getCell(this->position);
    shared_ptr<Link> link = nullptr;
    for (auto &occupant : cell.getOccupants()) {
      link = dynamic_pointer_cast<Link>(occupant);
      if (link)
        break;
    }

    if (link) {
      Position newPos = this->position;
      int boardSize = this->gameState.getBoard().getGridSize();

      if (this->position.getPosition().second == 0) {
        newPos.setPosition(this->position.getPosition().first, boardSize);
      } else {
        newPos.setPosition(this->position.getPosition().first, 1);
      }

      shared_ptr<Player> owner = link->permission.getOwner();

      if (this->gameState.getBoard().isValidPosition(
              newPos, owner->getPlayerNumber())) {
        this->gameState.removeOccupant(link, this->position);
        this->gameState.addOccupant(link, newPos);
        notifySideTriggered(link->getName(), newPos);
      }
    }
  };
}

void SideTrigger::notifySideTriggered(char link, Position &newPos) {
  auto queue = MessageQueue::getInstance();
  std::cout << "Link: " << link << std::endl;
  map<string, string> payloadMap = {
      {"occupant", string(1, link)},
      {"x", to_string(newPos.getPosition().first)},
      {"y", to_string(newPos.getPosition().second)}};

  Payload payload{payloadMap};
  EventType eventType = EventType::OccupantAdded;
  queue->enqueueEvent(GameEvent(eventType, payload));
}

SideTrigger::~SideTrigger() {}