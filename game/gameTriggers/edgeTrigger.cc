#include "edgeTrigger.h"
#include "../../game/cell.h"
#include "../../game/game_state.h"
#include "../../game/player.h"
#include "../../utils/payload.h"
#include <memory>

using namespace std;

EdgeTrigger::EdgeTrigger(GameState &gameState, const Position &pos,
                         const Permission &perm)
    : Trigger{gameState, pos, perm} {
  triggerAction = [this]() {
    Cell &cell = this->gameState.getBoard().getCell(this->position);
    shared_ptr<Link> link = nullptr;
    for (auto &occupant : cell.getOccupants()) {
      if (dynamic_pointer_cast<Link>(occupant)) {
        link = dynamic_pointer_cast<Link>(occupant);
        break;
      }
    }
    if (link) {
      if (permission.getOwner() != link->permission.getOwner()) {
        this->gameState.downloadLink(link, link->permission.getOwner());
      }
    }
  };
}

EdgeTrigger::~EdgeTrigger() {}