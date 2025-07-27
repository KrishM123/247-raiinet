#ifndef POLARIZE_H
#define POLARIZE_H

#include "ability.h"

class Polarize final : public Ability {
public:
    Polarize(GameState& gameState);
    ~Polarize() override;

    void execute(const Payload& payload) override;
};

#endif // POLARIZE_H 