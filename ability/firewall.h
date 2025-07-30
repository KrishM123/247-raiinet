#ifndef FIREWALL_H
#define FIREWALL_H

#include "ability.h"

class Firewall final : public Ability {
public:
  Firewall(Permission &permission, GameState &gameState);

  void execute(const Payload &payload) override;
};

#endif // FIREWALL_H
