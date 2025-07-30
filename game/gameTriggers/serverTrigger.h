#ifndef SERVER_TRIGGER_H
#define SERVER_TRIGGER_H

#include "../../ability/trigger.h"
#include <memory>

class ServerTrigger : public Trigger {
public:
  ServerTrigger(GameState &gameState, const Position &pos,
                const Permission &perm);
};

#endif // SERVER_TRIGGER_H
