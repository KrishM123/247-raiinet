#ifndef ABILITY_H
#define ABILITY_H

#include <memory>
#include <vector>
#include <string>
#include "../utils/permission.h"

class GameState;
class Payload;

class Ability {
public:
    Permission permission;
    bool used = false;
    std::string name;

    virtual ~Ability();
    virtual void execute(const Payload& payload) = 0;
    void notifyAbilityUsed();

protected:
    GameState& gameState;
    Ability(std::string name, Permission& permission, GameState& gameState);
};

#endif // ABILITY_H