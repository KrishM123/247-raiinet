#include "../ability/barbed_wire.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "trigger.h"
#include "../game/board.h"
#include "../game/cell.h"
#include "../utils/payload.h"
#include "../utils/position.h"
#include <string>
#include <sstream>

using namespace std;

// Constructor and Destructor
BarbedWire::BarbedWire(Permission& permission, GameState& gameState) :
    Ability("B", permission, gameState) {}

BarbedWire::~BarbedWire() {}

void BarbedWire::execute(const Payload& payload) {
    // Input:
    // String of two integers representing row and column.
    // Example: "2 5"
    
    string args = payload.get("args");
    stringstream ss(args);
    string rowStr, colStr;

    ss >> rowStr >> colStr;

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
        
        // TODO: Check if the target cell is empty and not a server port

        // Define the lambda function for the Barbed Wire's action
        auto barbed_wire_action = [this, targetPos](const Payload& payload) {
            // Find the link that triggered this action
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

            if (!triggeredLink) return;

            // Reveal the link to all players
            vector<shared_ptr<Player>> allPlayers = this->gameState.getPlayers();
            triggeredLink->permission.setVisibleTo(allPlayers);

            // Reset the link's strength based on its type
            if (triggeredLink->getType() == 0) { // 0 == Data
                triggeredLink->setStrength(1);
            } else if (triggeredLink->getType() == 1) { // 1 == Virus
                triggeredLink->setStrength(4);
            }
        };

        // Create a Trigger with the defined action
        auto trigger = make_shared<Trigger>(targetPos, this->permission, barbed_wire_action);

        // Place the trigger on the board
        gameState.addOccupant(trigger, targetPos);

        notifyAbilityUsed();

    } catch (const std::exception& e) {
        return; // Silently fail on bad input
    }
} 