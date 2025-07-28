#include "barbed_wire.h"
#include "../game/game_state.h"
#include "../utils/payload.h"

using namespace std;

BarbedWire::BarbedWire(Permission& permission, GameState& gameState) :
    Ability("B", permission, gameState) {}

BarbedWire::~BarbedWire() {}

void BarbedWire::execute(const Payload& payload) {
    // TODO: Implement Barbed Wire logic
    // 1. Parse coordinates from payload.get("args")
    // 2. Create a lambda for the Barbed Wire effect (reset strength)
    // 3. Create a Trigger with the lambda
    // 4. Add the Trigger to the board
    notifyAbilityUsed();
} 