#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include "../utils/payload.h"
#include "event_types.h"

class GameEvent final {
  EventType eventType;
  Payload payload;

public:
  GameEvent(EventType &eventType, Payload &payload);
  ~GameEvent();
  EventType getEventType();
  Payload &getPayload();
  void setPayload(const Payload &payload);
};

#endif // GAME_EVENT_H