#ifndef SCAN_H
#define SCAN_H

#include "ability.h"

class Scan : public Ability {
public:
    using Ability::Ability;
    void execute(const std::vector<std::string>& args) override;
};

#endif // SCAN_H 