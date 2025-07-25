#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "ability.h"

class Download final : public Ability {
public:
    Download(GameState& gameState);
    ~Download() override;

    void execute(const std::vector<std::string>& args) override;
};

#endif // DOWNLOAD_H 