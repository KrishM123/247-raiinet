#ifndef SWAPLINKS_H
#define SWAPLINKS_H

#include "ability.h"

class Swaplinks : public Ability {
public:
    using Ability::Ability;
    void execute(const std::vector<std::string>& args) override;
};

#endif // SWAPLINKS_H 