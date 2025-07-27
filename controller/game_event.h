#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include "event_types.h"
#include "../utils/payload.h"

class GameEvent final {
    EventType eventType;
    Payload payload;
public:
    GameEvent();
    ~GameEvent();
    EventType getEventType() const;
    Payload& getPayload();
    void setPayload(const Payload& payload);
};

#endif // GAME_EVENT_H