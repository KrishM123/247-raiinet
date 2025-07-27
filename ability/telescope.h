#ifndef TELESCOPE_H
#define TELESCOPE_H

#include "ability.h"

class Telescope : public Ability {
public:
    using Ability::Ability;
    void execute(const Payload& payload) override;
};

#endif // TELESCOPE_H 