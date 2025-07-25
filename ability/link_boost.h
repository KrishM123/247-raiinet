#ifndef LINK_BOOST_H
#define LINK_BOOST_H

#include "ability.h"

class LinkBoost final : public Ability {
public:
    LinkBoost(GameState& gameState);
    ~LinkBoost() override;

    void execute(const std::vector<std::string>& args) override;
};

#endif // LINK_BOOST_H 