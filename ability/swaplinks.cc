#include "../ability/swaplinks.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../game/link.h"
#include "trigger.h" // Corrected path
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
    // --- Input Format ---
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
    
    // Helper lambda to resolve all interactions at a new position
    auto resolveMove = [&](shared_ptr<Link> movingLink, Position newPos) {
        if (!movingLink) return movingLink;

        Cell& newCell = board.getCell(newPos);
        vector<shared_ptr<Occupant>> occupants = newCell.getOccupants();

        // 1. Trigger traps first
        for (auto& occupant : occupants) {
            if (auto trigger = dynamic_pointer_cast<Trigger>(occupant)) {
                if (trigger->permission.getOwner().get() != &currentPlayer) {
                    trigger->trigger(Payload{});
                }
            }
        }

        // 2. Battle opponent links
        shared_ptr<Link> opponentLink = nullptr;
        for (auto& occupant : occupants) {
            if (auto oLink = dynamic_pointer_cast<Link>(occupant)) {
                if (oLink->permission.getOwner().get() != &currentPlayer) {
                    opponentLink = oLink;
                    break;
                }
            }
        }
        
        if (opponentLink) {
            Player& opponent = *opponentLink->permission.getOwner();
            if (movingLink->getStrength() >= opponentLink->getStrength()) {
                board.removeOccupant(opponentLink, newPos);
                // (get/set logic to download link for current player)
                vector<shared_ptr<Link>> downloads = currentPlayer.getDownloadedLinks();
                downloads.push_back(opponentLink);
                currentPlayer.setDownloadedLinks(downloads);
            } else {
                board.removeOccupant(movingLink, movingLink->getPosition());
                // (get/set logic to download link for opponent)
                vector<shared_ptr<Link>> downloads = opponent.getDownloadedLinks();
                downloads.push_back(movingLink);
                opponent.setDownloadedLinks(downloads);
                return shared_ptr<Link>(nullptr); // movingLink was destroyed
            }
        }
        
        board.removeOccupant(movingLink, movingLink->getPosition());
        board.placeOccupant(movingLink, newPos);
        movingLink->setPosition(newPos);
        return movingLink;
    };

    // Execute the swap, re-assigning link pointers in case they are destroyed
    link1 = resolveMove(link1, pos2);
    link2 = resolveMove(link2, pos1);

    notifyAbilityUsed();
} 