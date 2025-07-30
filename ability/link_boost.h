#ifndef LINK_BOOST_H
#define LINK_BOOST_H

#include "ability.h"

class LinkBoost final : public Ability {
public:
  LinkBoost(Permission &permission, GameState &gameState);

  void execute(const Payload &payload) override;
};

#endif // LINK_BOOST_H
