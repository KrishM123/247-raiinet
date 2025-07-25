#ifndef BARBED_WIRE_H
#define BARBED_WIRE_H

#include "ability.h"

class BarbedWire : public Ability {
public:
    using Ability::Ability;
    void execute(const std::vector<std::string>& args) override;
};

#endif // BARBED_WIRE_H 