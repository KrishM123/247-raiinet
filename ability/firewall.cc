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
    // Input: string of two integers (row, col)
    // Example: "3 4"
    
    string args = payload.get("args");
    stringstream ss(args);
    string rowStr, colStr;

    ss >> rowStr >> colStr;

    // Exactly 2 args
    if (rowStr.empty() || colStr.empty() || !ss.eof()) {
        return; 
    }
    
    try {
        int row = stoi(rowStr);
        int col = stoi(colStr);
        int gridSize = gameState.getBoard().getGridSize();

        if (row < 0 || row >= gridSize || col < 0 || col >= gridSize) {
            return; // Out of bounds
        }

        Position targetPos{row, col};
        
        // Define the lambda function for the firewall's action
        auto firewall_action = [this, targetPos]() {
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
                    this->gameState.downloadLink(triggeredLink, owner);
                }
            }
        };

        // Create a Trigger with the defined action, providing the necessary
        // Position and the Permission from the ability itself.
        auto trigger = make_shared<Trigger>(gameState, targetPos, this->permission, firewall_action);

        // Place the trigger on the board
        gameState.addOccupant(trigger, targetPos);

        notifyAbilityUsed();
    } catch (const std::exception& e) {
        return;
    }
} 