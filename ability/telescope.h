#ifndef TELESCOPE_H
#define TELESCOPE_H

#include "ability.h"

class Telescope final : public Ability {
public:
    std::string name = "T";

    Telescope(Permission& permission, GameState& gameState);
    ~Telescope() override;

    void execute(const Payload& payload) override;
    std::string getName() override;
};

#endif // TELESCOPE_H 