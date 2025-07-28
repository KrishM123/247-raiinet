#include "../ability/firewall.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../game/link.h"
#include "trigger.h" // Corrected path
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

        // TODO: We need a way to get the board dimensions to check for out-of-bounds.
        // Assuming 8x8 for now.
        if (row < 0 || row >= 8 || col < 0 || col >= 8) {
            return; // Out of bounds
        }

        Position targetPos{row, col};
        
        // TODO: We need a way to check if the target cell is empty.
        // Assuming it's always empty for now.

        // Define the lambda function for the firewall's action
        auto firewall_action = [this](Link& triggeredLink) {
            // Reveal the link to all players
            vector<shared_ptr<Player>> allPlayers = this->gameState.getPlayers();
            triggeredLink.permission.setVisibleTo(allPlayers);

            // If the link is a virus, its owner downloads it
            if (triggeredLink.getType() == 1) { // 1 == Virus
                shared_ptr<Player> owner = triggeredLink.permission.getOwner();
                if (owner) {
                    // Manually implement the download logic using get/set
                    vector<shared_ptr<Link>> ownerDownloads = owner->getDownloadedLinks();
                    ownerDownloads.push_back(make_shared<Link>(triggeredLink));
                    owner->setDownloadedLinks(ownerDownloads);

                    // Also remove from owner's active links
                    vector<shared_ptr<Link>> ownerLinks = owner->getLinks();
                    for (auto it = ownerLinks.begin(); it != ownerLinks.end(); ++it) {
                        if (it->get() == &triggeredLink) {
                            ownerLinks.erase(it);
                            break;
                        }
                    }
                    owner->setLinks(ownerLinks);

                    // Finally, remove from the board
                    this->gameState.removeOccupant(make_shared<Link>(triggeredLink), triggeredLink.getPosition());
                }
            }
        };

        // Create a Trigger with the defined action
        auto trigger = make_shared<Trigger>(firewall_action);

        // Place the trigger on the board
        gameState.addOccupant(trigger, targetPos);

        notifyAbilityUsed();
    } catch (const std::exception& e) {
        // This will catch errors from stoi if the arguments are not valid integers.
        // Silently fail on bad input.
        return;
    }
} 