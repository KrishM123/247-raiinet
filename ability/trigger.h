#ifndef TRIGGER_H
#define TRIGGER_H

#include "../game/occupant.h"
#include <functional>

class Link;

class Trigger final : public Occupant {
public:
    Trigger(std::function<void(Link&)> action);
    ~Trigger() override;

    void trigger(Link& triggeredBy);

private:
    std::function<void(Link&)> triggerAction;
    void notifyRuleUsed();
};

#endif // TRIGGER_H