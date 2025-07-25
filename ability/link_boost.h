#ifndef LINK_BOOST_H
#define LINK_BOOST_H

#include "ability.h"

class LinkBoost : public Ability {
public:
    using Ability::Ability;
    void execute(const std::vector<std::string>& args) override;
};

#endif // LINK_BOOST_H 