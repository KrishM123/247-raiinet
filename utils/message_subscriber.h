#ifndef MESSAGE_SUBSCRIBER_H
#define MESSAGE_SUBSCRIBER_H

#include "game_event.h"

class MessageSubscriber {
public:
  virtual ~MessageSubscriber() = default;
  virtual void notify(GameEvent &event) = 0;
};

#endif // MESSAGE_SUBSCRIBER_H
