#ifndef SIDE_TRIGGER_H
#define SIDE_TRIGGER_H

#include <memory>

#include "../../ability/trigger.h"
#include "../../utils/position.h"

class GameState;
class Occupant;

class SideTrigger : public Trigger {
public:
  SideTrigger(GameState &gameState, const Position &pos);
  void notifySideTriggered(char winner, Position &newPos);
  ~SideTrigger();
};

#endif
