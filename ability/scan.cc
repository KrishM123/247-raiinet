#include "../ability/scan.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../game/link.h"
#include "../utils/payload.h"
#include "../utils/permission.h"
#include <vector>
#include <utility>
#include <sstream> // Added for stringstream

using namespace std;

// Helper function to parse the link character into player and link indices
// This can be easily extended for more players (e.g., 'i'-'p' for player 2)
pair<int, int> getScanTargetIndices(char linkId) {
    if (linkId >= 'a' && linkId <= 'h') {
        return {0, linkId - 'a'};
    }
    if (linkId >= 'A' && linkId <= 'H') {
        return {1, linkId - 'A'};
    }
    return {-1, -1};
}

// Constructor and Destructor
Scan::Scan(Permission& permission, GameState& gameState) :
    Ability("S", permission, gameState) {}

Scan::~Scan() {}

void Scan::execute(const Payload& payload) {
    // --- Input Format ---
    // This ability expects a string containing a single character link identifier.
    // Example: "A"

    string args = payload.get("args");
    stringstream ss(args);
    string linkIdStr;

    ss >> linkIdStr;

    // Check if we have exactly one argument
    if (linkIdStr.length() != 1 || !ss.eof()) {
        return; // Silently fail on incorrect input
    }
    char linkId = linkIdStr[0];

    // --- The rest of the logic remains the same ---
    pair<int, int> target = getScanTargetIndices(linkId);
    int targetPlayerIndex = target.first;
    int targetLinkIndex = target.second;

    if (targetPlayerIndex == -1) return;

    vector<shared_ptr<Player>> allPlayers = gameState.getPlayers();
    if (targetPlayerIndex >= allPlayers.size()) return;
    
    shared_ptr<Player> targetPlayer = allPlayers[targetPlayerIndex];
    vector<shared_ptr<Link>> targetPlayerLinks = targetPlayer->getLinks();
    
    if (targetLinkIndex >= targetPlayerLinks.size()) return;
    shared_ptr<Link> targetLink = targetPlayerLinks[targetLinkIndex];

    // Find the shared_ptr for the current player
    Player& currentPlayer = gameState.getCurPlayer();
    shared_ptr<Player> currentPlayerPtr = nullptr;
    for(const auto& p : allPlayers) {
        if (p.get() == &currentPlayer) {
            currentPlayerPtr = p;
            break;
        }
    }

    // The core logic: add the current player to the link's visibility list
    if(currentPlayerPtr) {
        targetLink->permission.addViewer(currentPlayerPtr);
    }

    notifyAbilityUsed();
} 