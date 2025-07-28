#include "../ability/download.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/payload.h"
#include <vector>
#include <utility>
#include <sstream>

using namespace std;

// Constructor and Destructor
Download::Download(Permission& permission, GameState& gameState) :
    Ability("D", permission, gameState) {}

Download::~Download() {}


// Helper function to parse the link character into player and link indices
// This can be easily extended for more players (e.g., 'i'-'p' for player 2)
pair<int, int> getTargetIndices(char linkId) {
    if (linkId >= 'a' && linkId <= 'h') {
        return {0, linkId - 'a'}; // Player 0, link index
    }
    if (linkId >= 'A' && linkId <= 'H') {
        return {1, linkId - 'A'}; // Player 1, link index
    }
    return {-1, -1}; // Invalid ID
}


void Download::execute(const Payload& payload) {
    // --- Input Format ---
    // This ability expects a string containing a single character link identifier.
    // Example: "C"
    
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
    pair<int, int> target = getTargetIndices(linkId);
    int targetPlayerIndex = target.first;
    int targetLinkIndex = target.second;

    if (targetPlayerIndex == -1) return; // Invalid link ID

    Player& currentPlayer = gameState.getCurPlayer();
    vector<shared_ptr<Player>> allPlayers = gameState.getPlayers();

    // Ensure target player exists
    if (targetPlayerIndex >= allPlayers.size()) return;

    Player& targetPlayer = *allPlayers[targetPlayerIndex];

    // A player cannot download their own link
    if (&currentPlayer == &targetPlayer) return;

    // Get the target link from the player's list of links
    vector<shared_ptr<Link>> targetPlayerLinks = targetPlayer.getLinks();
    if (targetLinkIndex >= targetPlayerLinks.size()) return; // Invalid link index
    shared_ptr<Link> targetLink = targetPlayerLinks[targetLinkIndex];


    // --- Perform the Download ---

    // 1. Remove the link from the target player's active links
    targetPlayerLinks.erase(targetPlayerLinks.begin() + targetLinkIndex);
    targetPlayer.setLinks(targetPlayerLinks);

    // 2. Add the link to the current player's downloaded list
    vector<shared_ptr<Link>> currentPlayerDownloads = currentPlayer.getDownloadedLinks();
    currentPlayerDownloads.push_back(targetLink);
    currentPlayer.setDownloadedLinks(currentPlayerDownloads);

    // 3. Update the link's internal state
    targetLink->setIsDownloaded(true);
    
    // 4. Remove the occupant from the board
    // Assumption: Link inherits getPosition() from Occupant
    gameState.removeOccupant(targetLink, targetLink->getPosition());

    notifyAbilityUsed();
} 