#include "link_boost.h"
#include "../game/game_state.h"
#include "../utils/payload.h"

using namespace std;

LinkBoost::LinkBoost(Permission& permission, GameState& gameState) :
    Ability("L", permission, gameState) {}

LinkBoost::~LinkBoost() {}

void LinkBoost::execute(const Payload& payload) {
    // TODO: Implement Link Boost logic
    // 1. Parse target link char from payload.get("args")
    // 2. Find the link object
    // 3. Set a "isBoosted" flag on the link object
    notifyAbilityUsed();
} 