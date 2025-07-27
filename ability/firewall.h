#ifndef FIREWALL_H
#define FIREWALL_H

#include "ability.h"

class Firewall : public Ability {
public:
    using Ability::Ability;
    void execute(const Payload& payload) override;
};

#endif // FIREWALL_H    