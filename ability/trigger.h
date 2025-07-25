#ifndef TRIGGER_H
#define TRIGGER_H

#include "../game/occupant.h"

// Forward-declaration
class Player;

class Trigger : public Occupant {
public:
    using Occupant::Occupant; // Inherit constructor

    void trigger(Player& triggeredBy);

private:
    void notifyRuleUsed();
};

#endif // TRIGGER_H