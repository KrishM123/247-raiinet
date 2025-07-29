#include "../ability/polarize.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../game/link.h"
#include "../utils/payload.h"
#include <string>
#include <sstream>

using namespace std;

// Helper:
// Parse link character into player and link indices
pair<int, int> getPolarizeTargetIndices(char linkId) {
    if (linkId >= 'a' && linkId <= 'h') {
        return {0, linkId - 'a'};
    }
    if (linkId >= 'A' && linkId <= 'H') {
        return {1, linkId - 'A'};
    }
    return {-1, -1};
}

Polarize::Polarize(Permission& permission, GameState& gameState) :
    Ability("P", permission, gameState) {}

Polarize::~Polarize() {}

void Polarize::execute(const Payload& payload) {
    // Input: string (single char) representing a link
    // Example: "d"
    
    string args = payload.get("args");
    stringstream ss(args);
    string linkIdStr;

    ss >> linkIdStr;

    if (linkIdStr.length() != 1 || !ss.eof()) {
        return; 
    }   
    
    char linkId = linkIdStr[0];

    pair<int, int> target = getPolarizeTargetIndices(linkId);
    int targetPlayerIndex = target.first;
    int targetLinkIndex = target.second;

    if (targetPlayerIndex == -1) return;
    
    shared_ptr<Player> targetPlayer = gameState.getPlayers()[targetPlayerIndex];
    vector<shared_ptr<Link>> targetPlayerLinks = targetPlayer->getLinks();
    
    if (targetLinkIndex >= targetPlayerLinks.size()) {
        return;
    }
    
    shared_ptr<Link> targetLink = targetPlayerLinks[targetLinkIndex];

    // Apply polarity change
    int currentType = targetLink->getType();
    if (currentType == 0) {
        targetLink->setType(1); // Change Data to Virus
    } else if (currentType == 1) {
        targetLink->setType(0); // Change Virus to Data
    }

    notifyAbilityUsed();
} 