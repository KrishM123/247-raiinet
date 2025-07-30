#include "game_event.h"

GameEvent::GameEvent(EventType &eventType, Payload &payload)
    : eventType{eventType}, payload{payload} {}

GameEvent::~GameEvent() {}

EventType GameEvent::getEventType() { return eventType; }

Payload &GameEvent::getPayload() { return payload; }

void GameEvent::setPayload(const Payload &payload) { this->payload = payload; }