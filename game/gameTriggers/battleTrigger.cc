#include "battleTrigger.h"
#include "../../utils/payload.h"
#include "../../utils/permission.h"
#include "../../game/game_state.h"
#include "../../game/player.h"
#include "../../game/cell.h"
#include <memory>

using namespace std;

BattleTrigger::BattleTrigger(GameState &gameState, const Position &pos) : Trigger{gameState, pos, Permission{}}
{
  triggerAction = [this]()
  {
    Cell &cell = this->gameState.getBoard().getCell(this->position);
    shared_ptr<Link> attacker = nullptr;
    shared_ptr<Link> defender = nullptr;
    for (auto &occupant : cell.getOccupants())
    {
      if (dynamic_pointer_cast<Link>(occupant))
      {
        shared_ptr<Link> link = dynamic_pointer_cast<Link>(occupant);
        if (defender)
        {
          attacker = link;
        }
        else
        {
          defender = link;
        }
      }
    }
    if (attacker && defender)
    {
      if (attacker->getStrength() >= defender->getStrength())
      {
        this->gameState.downloadLink(defender, attacker->permission.getOwner());
      }
      else
      {
        this->gameState.downloadLink(attacker, defender->permission.getOwner());
      }
    }
    if (this->gameState.isWon())
    {
      this->gameState.endGame();
    }
  };
}

BattleTrigger::~BattleTrigger() {}