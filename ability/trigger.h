#ifndef TRIGGER_H
#define TRIGGER_H

#include "../game/occupant.h"
#include <functional>

class Link;
class Payload;
class Position;
class Permission;

class Trigger : public Occupant {
public:
    Trigger(const Position& pos, const Permission& perm, std::function<void(const Payload&)> action);
    ~Trigger() override;

    void trigger(const Payload& payload);

private:
    std::function<void(const Payload&)> triggerAction;
    void notifyRuleUsed();
};

#endif // TRIGGER_H