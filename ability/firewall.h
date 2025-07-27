#ifndef FIREWALL_H
#define FIREWALL_H

#include "ability.h"

class Firewall final : public Ability {
public:
    Firewall(GameState& gameState);
    ~Firewall() override;

    void execute(const Payload& payload) override;
};

#endif // FIREWALL_H    