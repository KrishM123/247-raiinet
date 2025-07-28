#include "telescope.h"
#include "../game/game_state.h"
#include "../utils/payload.h"

using namespace std;

Telescope::Telescope(Permission& permission, GameState& gameState) :
    Ability("T", permission, gameState) {}

Telescope::~Telescope() {}

void Telescope::execute(const Payload& payload) {
    // TODO: Implement Telescope logic
    // 1. This ability takes no arguments from the payload.
    // 2. It should fire a GameEvent containing the opponent's ability list.
    // 3. The View will be responsible for catching this event and displaying the info.
    notifyAbilityUsed();
} 