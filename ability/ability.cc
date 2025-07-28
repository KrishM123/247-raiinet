#include "../ability/ability.h"
#include "../game/game_state.h"

using namespace std;

Ability::Ability(string name, Permission& permission, GameState& gameState) :
    name{name}, permission{permission}, gameState{gameState} {}

Ability::~Ability() {}

void Ability::notifyAbilityUsed() {
    this->used = true;
    this->permission.setVisibleTo(gameState.getPlayers());
    // TODO: Fire a GameEvent if necessary
} 