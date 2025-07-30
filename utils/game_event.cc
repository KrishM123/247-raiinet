#include "game_event.h"

GameEvent::GameEvent(EventType &eventType, Payload &payload)
    : eventType{eventType}, payload{payload} {}

EventType GameEvent::getEventType() { return eventType; }

Payload &GameEvent::getPayload() { return payload; }
