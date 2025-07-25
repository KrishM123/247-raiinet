#ifndef POLARIZE_H
#define POLARIZE_H

#include "ability.h"

class Polarize final : public Ability {
public:
    Polarize(GameState& gameState);
    ~Polarize() override;

    void execute(const std::vector<std::string>& args) override;
};

#endif // POLARIZE_H 