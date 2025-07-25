#ifndef FIREWALL_H
#define FIREWALL_H

#include "ability.h"

class Firewall : public Ability {
public:
    using Ability::Ability; // Inherit constructor
    void execute(const std::vector<std::string>& args) override;
};

#endif // FIREWALL_H 