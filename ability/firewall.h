#ifndef FIREWALL_H
#define FIREWALL_H

#include "ability.h"

class Firewall final : public Ability {
public:
    std::string name = "F";

    Firewall(Permission& permission, GameState& gameState);
    ~Firewall() override;

    void execute(const Payload& payload) override;
    std::string getName() override;
};

#endif // FIREWALL_H    