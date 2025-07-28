#include "battleTrigger.h"
#include "../../utils/payload.h"
#include "../../utils/permission.h"

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
  // TODO: Implement battle logic
}