#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "ability.h"

class Download : public Ability {
public:
    using Ability::Ability;
    void execute(const std::vector<std::string>& args) override;
};

#endif // DOWNLOAD_H 