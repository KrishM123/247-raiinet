#include "serverTrigger.h"
#include "../../game/cell.h"
#include "../../game/game_state.h"
#include "../../game/player.h"
#include <memory>

using namespace std;

ServerTrigger::ServerTrigger(GameState &gameState, const Position &pos,
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
        link = dynamic_pointer_cast<Link>(occupant);
        break;
      }
    }
    // If the link is found
    if (link) {
      // Get the owners of the server and the link
      shared_ptr<Player> serverOwner = permission.getOwner();
      shared_ptr<Player> linkOwner = link->permission.getOwner();
      // If the server's owner is not the same as the link's owner, download the link
      if (serverOwner != linkOwner) {
        this->gameState.downloadLink(link, serverOwner);
      }
    }
  };
}
