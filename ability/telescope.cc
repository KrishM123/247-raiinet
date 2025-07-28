#include "telescope.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include <vector>

using namespace std;

Telescope::Telescope(Permission& permission, GameState& gameState) :
    Ability("T", permission, gameState) {}

Telescope::~Telescope() {}

void Telescope::execute(const Payload& payload) {
    Player& currentPlayer = gameState.getCurPlayer();
    shared_ptr<Player> opponent = nullptr;
    shared_ptr<Player> currentPlayerPtr = nullptr;

    // Find both the opponent and the shared_ptr for the current player
    for (auto& player : gameState.getPlayers()) {
        if (player.get() != &currentPlayer) {
            opponent = player;
        } else {
            currentPlayerPtr = player;
        }
    }

    // If we found both, grant permission
    if (opponent && currentPlayerPtr) {
        for (auto& ability : opponent->getAbilities()) {
            ability->permission.addViewer(currentPlayerPtr);
        }
    }

    notifyAbilityUsed();
} 