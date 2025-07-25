#ifndef FIREWALL_H
#define FIREWALL_H

#include "ability.h"

class Firewall final : public Ability {
public:
    Firewall(GameState& gameState);
    ~Firewall() override;

    void execute(const std::vector<std::string>& args) override;
};

#endif // FIREWALL_H 