#ifndef BARBED_WIRE_H
#define BARBED_WIRE_H

#include "ability.h"

class BarbedWire final : public Ability {
public:
  BarbedWire(Permission &permission, GameState &gameState);
  ~BarbedWire() override;

  void execute(const Payload &payload) override;
};

#endif // BARBED_WIRE_H