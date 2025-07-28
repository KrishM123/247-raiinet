#include "swaplinks.h"
#include "../game/game_state.h"
#include "../utils/payload.h"

using namespace std;

Swaplinks::Swaplinks(Permission& permission, GameState& gameState) :
    Ability("S", permission, gameState) {}

Swaplinks::~Swaplinks() {}

void Swaplinks::execute(const Payload& payload) {
    // TODO: Implement Swap Links logic
    // This is the most complex ability.
    // 1. Parse two link chars from payload.get("args")
    // 2. Find both link objects
    // 3. Swap their positions
    // 4. For each link, check for and resolve battles or trap triggers
    notifyAbilityUsed();
} 