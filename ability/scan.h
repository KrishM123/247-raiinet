#ifndef SCAN_H
#define SCAN_H

#include "ability.h"

class Scan final : public Ability {
public:
    std::string name = "A";

    Scan(Permission& permission, GameState& gameState);
    ~Scan() override;

    void execute(const Payload& payload) override;
    std::string getName() override;
};

#endif // SCAN_H    