#include "edgeTrigger.h"
#include "../../game/cell.h"
#include "../../game/game_state.h"
#include <memory>

using namespace std;

EdgeTrigger::EdgeTrigger(GameState &gameState, const Position &pos,
                         const Permission &perm)
    : Trigger{gameState, pos, perm} {
  // Create a trigger action that will be executed when the trigger is triggered
  triggerAction = [this]() {
    // Get the cell at the trigger position
    Cell &cell = this->gameState.getBoard().getCell(this->position);
    shared_ptr<Link> link = nullptr;
    // Find the link in the cell
    for (auto &occupant : cell.getOccupants()) {
      if (dynamic_pointer_cast<Link>(occupant)) {
        // If the occupant is a link, set it as the link
        link = dynamic_pointer_cast<Link>(occupant);
        break;
      }
    }
    // If the link is found
    if (link) {
      // If the link's owner is not the same as the trigger's owner, download the link
      if (permission.getOwner() != link->permission.getOwner()) {
        this->gameState.downloadLink(link, link->permission.getOwner());
      }
    }
  };
}
