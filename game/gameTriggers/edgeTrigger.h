#ifndef EDGE_TRIGGER_H
#define EDGE_TRIGGER_H

#include "../../ability/trigger.h"
#include "../link.h"
#include <memory>

class EdgeTrigger : public Trigger {
public:
  EdgeTrigger(GameState &gameState, const Position &pos,
              const Permission &perm);
};

#endif // EDGE_TRIGGER_H
