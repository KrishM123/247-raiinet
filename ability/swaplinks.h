#ifndef SWAPLINKS_H
#define SWAPLINKS_H

#include "ability.h"

class Swaplinks final : public Ability {
public:
    Swaplinks(GameState& gameState);
    ~Swaplinks() override;

    void execute(const std::vector<std::string>& args) override;
};

#endif // SWAPLINKS_H 