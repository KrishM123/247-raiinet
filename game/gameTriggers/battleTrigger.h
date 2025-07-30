#ifndef BATTLE_TRIGGER_H
#define BATTLE_TRIGGER_H

#include "../../ability/trigger.h"
#include "../link.h"
#include <memory>

class BattleTrigger : public Trigger {
  void notifyBattleTriggered(char winner, Position &position);

public:
  BattleTrigger(GameState &gameState, const Position &pos);
};

#endif // BATTLE_TRIGGER_H
