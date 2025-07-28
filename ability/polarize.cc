#include "polarize.h"
#include "../game/game_state.h"
#include "../utils/payload.h"

using namespace std;

Polarize::Polarize(Permission& permission, GameState& gameState) :
    Ability("P", permission, gameState) {}

Polarize::~Polarize() {}

void Polarize::execute(const Payload& payload) {
    // TODO: Implement Polarize logic
    // 1. Parse target link char from payload.get("args")
    // 2. Find the link object
    // 3. Flip its type (0 to 1 or 1 to 0)
    notifyAbilityUsed();
} 