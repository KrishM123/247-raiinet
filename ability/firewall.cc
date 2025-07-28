#include "../ability/firewall.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../game/link.h"
#include "trigger.h" // Corrected path
#include "../game/board.h"
#include "../game/cell.h"
#include "../utils/payload.h"
#include "../utils/position.h"
#include <string>
#include <sstream>

using namespace std;

// Constructor and Destructor
Firewall::Firewall(Permission& permission, GameState& gameState) :
    Ability("F", permission, gameState) {}

Firewall::~Firewall() {}

void Firewall::execute(const Payload& payload) {
    // --- Input Format ---
    // This ability expects a string of two integers representing row and column.
    // Example: "3 4"
    
    string args = payload.get("args");
    stringstream ss(args);
    string rowStr, colStr;

    ss >> rowStr >> colStr;

    // Check if we have exactly two arguments
    if (rowStr.empty() || colStr.empty() || !ss.eof()) {
        return; // Silently fail on incorrect number of arguments
    }
    
    try {
        int row = stoi(rowStr);
        int col = stoi(colStr);
        int gridSize = gameState.getBoard().getGridSize();

        if (row < 0 || row >= gridSize || col < 0 || col >= gridSize) {
            return; // Out of bounds
        }

        Position targetPos{row, col};
        
        // TODO: We need a way to check if the target cell is empty.
        // Assuming it's always empty for now.

        // Define the lambda function for the firewall's action
        auto firewall_action = [this, targetPos](const Payload& payload) {
            // The firewall needs to find the link that triggered it.
            // It's the other link on the same cell that isn't owned by the firewall's owner.
            Cell& myCell = this->gameState.getBoard().getCell(targetPos);
            shared_ptr<Link> triggeredLink = nullptr;

            for (auto& occupant : myCell.getOccupants()) {
                if (auto link = dynamic_pointer_cast<Link>(occupant)) {
                    if (link->permission.getOwner().get() != this->permission.getOwner().get()) {
                        triggeredLink = link;
                        break;
                    }
                }
            }

            if (!triggeredLink) return; // Should not happen in normal gameplay

            // Reveal the link to all players
            vector<shared_ptr<Player>> allPlayers = this->gameState.getPlayers();
            triggeredLink->permission.setVisibleTo(allPlayers);

            // If the link is a virus, its owner downloads it
            if (triggeredLink->getType() == 1) { // 1 == Virus
                shared_ptr<Player> owner = triggeredLink->permission.getOwner();
                if (owner) {
                    // Manually implement the download logic using get/set
                    vector<shared_ptr<Link>> ownerDownloads = owner->getDownloadedLinks();
                    ownerDownloads.push_back(triggeredLink);
                    owner->setDownloadedLinks(ownerDownloads);

                    // Also remove from owner's active links
                    vector<shared_ptr<Link>> ownerLinks = owner->getLinks();
                    for (auto it = ownerLinks.begin(); it != ownerLinks.end(); ++it) {
                        if (*it == triggeredLink) {
                            ownerLinks.erase(it);
                            break;
                        }
                    }
                    owner->setLinks(ownerLinks);

                    // Finally, remove from the board
                    this->gameState.removeOccupant(triggeredLink, triggeredLink->getPosition());
                }
            }
        };

        // Create a Trigger with the defined action, providing the necessary
        // Position and the Permission from the ability itself.
        auto trigger = make_shared<Trigger>(targetPos, this->permission, firewall_action);

        // Place the trigger on the board
        gameState.addOccupant(trigger, targetPos);

        notifyAbilityUsed();
    } catch (const std::exception& e) {
        // This will catch errors from stoi if the arguments are not valid integers.
        // Silently fail on bad input.
        return;
    }
} 