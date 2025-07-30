#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "ability.h"

class Download final : public Ability {
public:
  Download(Permission &permission, GameState &gameState);

  void execute(const Payload &payload) override;
};

#endif // DOWNLOAD_H
