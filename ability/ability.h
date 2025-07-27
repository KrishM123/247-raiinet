#ifndef ABILITY_H
#define ABILITY_H

#include <memory>
#include <vector>
#include <string>

class Player;
class GameState;
class Payload;

class Ability {
protected:
    Ability(Player& owner, GameState& gameState);

public:
    virtual ~Ability() = default;

    virtual void execute(const Payload& payload) = 0;

    void notifyAbilityUsed();

private:
    GameState& gameState;
};

#endif // ABILITY_H