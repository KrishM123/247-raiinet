#ifndef TRIGGER_H
#define TRIGGER_H

#include "../game/occupant.h"

class Player;

class Trigger : public Occupant {
public:
    void trigger(Player& triggeredBy);

private:
    void notifyRuleUsed();
};

#endif // TRIGGER_H