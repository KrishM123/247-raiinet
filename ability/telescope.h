#ifndef TELESCOPE_H
#define TELESCOPE_H

#include "ability.h"

class Telescope final : public Ability {
public:
    Telescope(Permission& permission, GameState& gameState);
    ~Telescope() override;

    void execute(const Payload& payload) override;
};

#endif // TELESCOPE_H 