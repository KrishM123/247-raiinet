#include "battleTrigger.h"
#include "../../utils/payload.h"
#include "../../utils/permission.h"
#include "../../game/game_state.h"
#include "../../game/player.h"
#include <memory>

using namespace std;

BattleTrigger::BattleTrigger(GameState &gameState, const Position &pos) : Trigger{gameState, pos, Permission{}}
{
  triggerAction = [this](const Payload &payload)
  {
    handleBattle(payload);
  };
}

BattleTrigger::~BattleTrigger() {}

void BattleTrigger::handleBattle(const Payload &payload)
{
  shared_ptr<Link> attacker = gameState.getLink(payload.get("attacker")[0]);
  shared_ptr<Link> defender = gameState.getLink(payload.get("defender")[0]);

  shared_ptr<Player> attackerOwner = attacker->permission.getOwner();
  shared_ptr<Player> defenderOwner = defender->permission.getOwner();

  attacker->permission.addViewer(defenderOwner);
  defender->permission.addViewer(attackerOwner);

  if (attacker->getStrength() >= defender->getStrength())
  {
    gameState.downloadLink(defender, attackerOwner);
  }
  else
  {
    gameState.downloadLink(attacker, defenderOwner);
  }

  if (gameState.isWon())
  {
    gameState.endGame();
  }
}