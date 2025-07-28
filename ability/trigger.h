#ifndef TRIGGER_H
#define TRIGGER_H

#include "../game/occupant.h"
#include <functional>

class Link;
class Payload;
class Position;
class Permission;
class GameState;

class Trigger : public Occupant
{
protected:
    GameState &gameState;
    std::function<void(const Payload &)> triggerAction;

private:
    void notifyRuleUsed();

public:
    Trigger(GameState &gameState, const Position &pos, const Permission &perm);
    Trigger(GameState &gameState, const Position &pos, const Permission &perm, std::function<void(const Payload &)> action);
    ~Trigger() override = default;

    void trigger(const Payload &payload);
};

#endif // TRIGGER_H