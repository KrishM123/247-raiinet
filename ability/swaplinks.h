#ifndef SWAPLINKS_H
#define SWAPLINKS_H

#include "ability.h"

class Swaplinks final : public Ability {
public:
    std::string name = "S";

    Swaplinks(Permission& permission, GameState& gameState);
    ~Swaplinks() override;

    void execute(const Payload& payload) override;
    std::string getName() override;
};

#endif // SWAPLINKS_H