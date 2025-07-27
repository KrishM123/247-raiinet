#ifndef SWAPLINKS_H
#define SWAPLINKS_H

#include "ability.h"

class Swaplinks : public Ability {
public:
    using Ability::Ability;
    void execute(const Payload& payload) override;
};

#endif // SWAPLINKS_H