#ifndef LINK_BOOST_H
#define LINK_BOOST_H

#include "ability.h"

class LinkBoost : public Ability {
public:
    using Ability::Ability;
    void execute(const Payload& payload) override;
};

#endif // LINK_BOOST_H 