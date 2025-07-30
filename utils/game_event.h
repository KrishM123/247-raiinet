#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include "event_types.h"
#include "payload.h"

class GameEvent final {
  EventType eventType;
  Payload payload;

public:
  GameEvent(EventType &eventType, Payload &payload);
  EventType getEventType();
  Payload &getPayload();
};

#endif // GAME_EVENT_H
