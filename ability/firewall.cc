#include "../ability/firewall.h"
#include "../game/board.h"
#include "../game/cell.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/payload.h"
#include "../utils/position.h"
#include "trigger.h"
#include <sstream>
#include <string>

using namespace std;

Firewall::Firewall(Permission &permission, GameState &gameState)
    : Ability("F", permission, gameState) {}

void Firewall::execute(const Payload &payload) {
  // Input: string of two integers (row, col)
  // Example: "3 4"

  string args = payload.get("args");
  stringstream ss(args);
  string rowStr, colStr;

  ss >> rowStr >> colStr;

  // Exactly 2 args
  if (rowStr.empty() || colStr.empty()) {
    return;
  }

  try {
    int row = stoi(rowStr) + 1;
    int col = stoi(colStr) + 1;
    int gridSize = gameState.getBoard().getGridSize();

    if (row < 0 || row > gridSize || col < 0 || col > gridSize) {
      return;
    }

    Position targetPos{row, col};

    Cell &targetCell = gameState.getBoard().getCell(targetPos);
    if (targetCell.getOccupants().size() > 1 || targetCell.getType() == 1 ||
        targetCell.getType() == 2) {
      throw invalid_argument("Position is not empty");
    }

    auto firewallAction = [this, targetPos]() {
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

      if (!triggeredLink) {
        return;
      }

      // Reveal the link to all players
      vector<shared_ptr<Player>> allPlayers = this->gameState.getPlayers();
      triggeredLink->permission.setVisibleTo(allPlayers);

      // If the link is a virus (Virus = 1), its owner downloads it
      if (triggeredLink->getType() == 1) {
        shared_ptr<Player> owner = triggeredLink->permission.getOwner();

        if (owner) {
          this->gameState.downloadLink(triggeredLink, owner);
        }
      }
    };

    // Create a Trigger with the defined action, providing the necessary
    // Position and the Permission from the ability itself.
    auto trigger = make_shared<Trigger>(gameState, targetPos, this->permission,
                                        firewallAction);

    // Place the trigger on the board
    gameState.addOccupant(trigger, targetPos);

    string marker;
    if (permission.getOwner()->getPlayerNumber() == 1) {
      marker = "m";
    } else if (permission.getOwner()->getPlayerNumber() == 2) {
      marker = "w";
    } else {
      marker = "-";
    }

    notifyAbilityUsed();
    notifyAbilityPlaced(targetPos, marker);
  } catch (const std::exception &e) {
    throw;
  }
}
