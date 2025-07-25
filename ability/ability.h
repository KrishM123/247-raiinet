#ifndef ABILITY_H
#define ABILITY_H

#include <memory>
#include <vector>
#include <string>

// Forward-declarations
class Player;
class GameState;

class Ability {
protected:
    // Only allow subclasses to instantiate
    Ability(GameState& gameState);

public:
    virtual ~Ability() = default;

    // Pure virtual function makes this an abstract class
    virtual void execute(const std::vector<std::string>& args) = 0;

    void notifyAbilityUsed();

private:
    GameState& gameState;
};

#endif // ABILITY_H