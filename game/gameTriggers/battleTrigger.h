#ifndef BATTLE_TRIGGER_H
#define BATTLE_TRIGGER_H

#include "../../ability/trigger.h"
#include "../link.h"
#include <memory>

class BattleTrigger : public Trigger {
public:
  BattleTrigger(GameState &gameState, const Position &pos);
  ~BattleTrigger() override;
};

#endif // BATTLE_TRIGGER_H
