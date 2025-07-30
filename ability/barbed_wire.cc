#include "../ability/barbed_wire.h"
#include "../game/board.h"
#include "../game/cell.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../utils/payload.h"
#include "../utils/position.h"
#include "trigger.h"
#include <sstream>
#include <string>

using namespace std;

BarbedWire::BarbedWire(Permission &permission, GameState &gameState)
    : Ability("B", permission, gameState) {}

void BarbedWire::execute(const Payload &payload) {
  // Input:
  // String of two integers representing row and column.
  // Example: "2 5"

  string args = payload.get("args");
  stringstream ss(args);
  string r, c;

  ss >> r >> c;

  if (r.empty() || c.empty()) {
    throw invalid_argument("Invalid payload");
  }

  try {
    int row = stoi(r) + 1;
    int col = stoi(c) + 1;
    int gridSize = gameState.getBoard().getGridSize();

    if (row <= 0 || row > gridSize || col <= 0 || col > gridSize) {
      throw invalid_argument("Invalid coordinates");
    }

    Position targetPos{row, col};

    Cell &targetCell = gameState.getBoard().getCell(targetPos);
    if (targetCell.getOccupants().size() > 1 || targetCell.getType() == 1 ||
        targetCell.getType() == 2) {
      throw invalid_argument("Position is not empty");
    }

    // Lambda function for barbed wire
    auto barbedWireAction = [this, targetPos]() {
      // Find the link that triggered this action
      Cell &myCell = this->gameState.getBoard().getCell(targetPos);
      shared_ptr<Link> triggeredLink = nullptr;

      for (auto &occupant : myCell.getOccupants()) {
        if (auto link = dynamic_pointer_cast<Link>(occupant)) {
          if (link->permission.getOwner().get() !=
              this->permission.getOwner().get()) {
            triggeredLink = link;
            break;
          }
        }
      }

      if (!triggeredLink)
        return;

      // Reveal the link to all players
      vector<shared_ptr<Player>> allPlayers = this->gameState.getPlayers();
      triggeredLink->permission.setVisibleTo(allPlayers);

      // Reset the link's strength based on its type (Virus = 1, Data = 0)
      if (triggeredLink->getType() == 0) {
        triggeredLink->setStrength(1);
      } else if (triggeredLink->getType() == 1) {
        triggeredLink->setStrength(4);
      }
    };

    // Create a Trigger with the defined action
    auto trigger = make_shared<Trigger>(gameState, targetPos, this->permission,
                                        barbedWireAction);

    // Place the trigger on the board
    gameState.addOccupant(trigger, targetPos);

    notifyAbilityPlaced(targetPos, "x");

    notifyAbilityUsed();

  } catch (const std::exception &e) {
    throw;
  }
}
