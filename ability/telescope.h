#ifndef TELESCOPE_H
#define TELESCOPE_H

#include "ability.h"

class Telescope final : public Ability {
public:
    Telescope(GameState& gameState);
    ~Telescope() override;

    void execute(const std::vector<std::string>& args) override;
};

#endif // TELESCOPE_H 