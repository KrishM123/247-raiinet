#ifndef ABILITY_H
#define ABILITY_H

#include <memory>
#include <vector>
#include <string>
#include "../utils/permission.h"

class GameState;
class Payload;

class Ability {
    Permission permission;
    GameState& gameState;

protected:
    Ability(Permission& permission, GameState& gameState);

public:
    virtual ~Ability() = default;

    virtual void execute(const Payload& payload) = 0;
    Permission getPermission() const;
    void setPermission(const Permission& permission);

    void notifyAbilityUsed();

};

#endif // ABILITY_H