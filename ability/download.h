#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "ability.h"

class Download final : public Ability {
public:
    std::string name = "D";

    Download(Permission& permission, GameState& gameState);
    ~Download() override;

    void execute(const Payload& payload) override;
    std::string getName() override;
};

#endif // DOWNLOAD_H 