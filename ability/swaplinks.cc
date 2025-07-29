#include "../ability/swaplinks.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../game/link.h"
#include "trigger.h" 
#include "../game/board.h"
#include "../game/cell.h"
#include "../utils/payload.h"
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// Helper function to find a player's link by its character ID
shared_ptr<Link> findLinkById(Player& player, char linkId) {
    int playerNum = player.getPlayerNumber();
    int linkIndex = -1;
    if (playerNum == 0 && linkId >= 'a' && linkId <= 'h') {
        linkIndex = linkId - 'a';
    } else if (playerNum == 1 && linkId >= 'A' && linkId <= 'H') {
        linkIndex = linkId - 'A';
    }
    
    if (linkIndex != -1 && linkIndex < player.getLinks().size()) {
        return player.getLinks()[linkIndex];
    }
    return nullptr;
}

Swaplinks::Swaplinks(Permission& permission, GameState& gameState) :
    Ability("S", permission, gameState) {}

Swaplinks::~Swaplinks() {}

void Swaplinks::execute(const Payload& payload) {
    // Input:
    // Example: "a d"
    string args = payload.get("args");
    stringstream ss(args);
    string linkId1Str, linkId2Str;
    ss >> linkId1Str >> linkId2Str;

    if (linkId1Str.length() != 1 || linkId2Str.length() != 1 || !ss.eof()) return;
    
    Player& currentPlayer = gameState.getCurPlayer();
    shared_ptr<Link> link1 = findLinkById(currentPlayer, linkId1Str[0]);
    shared_ptr<Link> link2 = findLinkById(currentPlayer, linkId2Str[0]);

    if (!link1 || !link2 || link1 == link2) return;

    Position pos1 = link1->getPosition();
    Position pos2 = link2->getPosition();
    Board& board = gameState.getBoard();
    
    // Swap
    // Remove both links from their current positions
    board.removeOccupant(link1, pos1);
    board.removeOccupant(link2, pos2);

    // Place both links in their new positions
    board.placeOccupant(link1, pos2);
    board.placeOccupant(link2, pos1);

    // Update internal position trackers of links
    link1->setPosition(pos2);
    link2->setPosition(pos1);

    notifyAbilityUsed();
} 