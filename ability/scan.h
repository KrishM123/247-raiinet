#ifndef SCAN_H
#define SCAN_H

#include "ability.h"

class Scan final : public Ability {
public:
  Scan(Permission &permission, GameState &gameState);

  void execute(const Payload &payload) override;
};

#endif // SCAN_H
