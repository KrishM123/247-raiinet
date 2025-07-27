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

protected:
    GameState& gameState;
    Ability(Permission& permission, GameState& gameState);

public:
    virtual ~Ability();

    virtual void execute(const Payload& payload) = 0;
    
    void notifyAbilityUsed();

};

#endif // ABILITY_H