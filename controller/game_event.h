#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include <map>
#include <string>
#include "event_types.h"

class GameEvent final {
    EventType eventType;
    std::map<std::string, int> payload;
public:
    GameEvent();
    ~GameEvent();
    EventType getEventType() const;
    std::map<std::string, int>& getPayload();
    void setPayload(const std::map<std::string, int>& payload);
};

#endif // GAME_EVENT_H