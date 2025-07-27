#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "ability.h"

class Download : public Ability {
public:
    using Ability::Ability;
    void execute(const Payload& payload) override;
};

#endif // DOWNLOAD_H 