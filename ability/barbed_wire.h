#ifndef BARBED_WIRE_H
#define BARBED_WIRE_H

#include "ability.h"

class BarbedWire final : public Ability {
public:
    BarbedWire(GameState& gameState);
    ~BarbedWire() override;

    void execute(const std::vector<std::string>& args) override;
};

#endif // BARBED_WIRE_H 