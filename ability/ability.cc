#include "../ability/ability.h"
#include "../game/game_state.h"

using namespace std;

Ability::Ability(Permission& permission, GameState& gameState) :
    permission{permission}, gameState{gameState} {}

Ability::~Ability() {}

void Ability::notifyAbilityUsed() {
    // Implement this later
} 