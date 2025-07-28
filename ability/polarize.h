#ifndef POLARIZE_H
#define POLARIZE_H

#include "ability.h"

class Polarize final : public Ability {
public:
    std::string name = "P";

    Polarize(Permission& permission, GameState& gameState);
    ~Polarize() override;

    void execute(const Payload& payload) override;
    std::string getName() override;
};

#endif // POLARIZE_H 