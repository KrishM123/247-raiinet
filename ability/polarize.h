#ifndef POLARIZE_H
#define POLARIZE_H

#include "ability.h"

class Polarize : public Ability {
public:
    using Ability::Ability;
    void execute(const std::vector<std::string>& args) override;
};

#endif // POLARIZE_H 