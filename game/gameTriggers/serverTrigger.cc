#include "serverTrigger.h"
#include "../../utils/payload.h"
#include "../../game/game_state.h"
#include "../../game/player.h"
#include "../../game/cell.h"
#include <memory>

using namespace std;

ServerTrigger::ServerTrigger(GameState &gameState, const Position &pos, const Permission &perm) : Trigger{gameState, pos, perm}
{
  triggerAction = [this]()
  {
    Cell &cell = this->gameState.getBoard().getCell(this->position);
    shared_ptr<Link> link = nullptr;
    for (auto &occupant : cell.getOccupants())
    {
      if (dynamic_pointer_cast<Link>(occupant))
      {
        link = dynamic_pointer_cast<Link>(occupant);
        break;
      }
    }
    if (link)
    {
      shared_ptr<Player> serverOwner = permission.getOwner();
      shared_ptr<Player> linkOwner = link->permission.getOwner();
      if (serverOwner != linkOwner)
      {
        this->gameState.downloadLink(link, serverOwner);
      }
    }
  };
}

ServerTrigger::~ServerTrigger() {}