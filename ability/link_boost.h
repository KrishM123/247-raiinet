#ifndef LINK_BOOST_H
#define LINK_BOOST_H

#include "ability.h"

class LinkBoost final : public Ability {
public:
    std::string name = "L";

    LinkBoost(Permission& permission, GameState& gameState);
    ~LinkBoost() override;

    void execute(const Payload& payload) override;
    std::string getName() override;
};

#endif // LINK_BOOST_H 