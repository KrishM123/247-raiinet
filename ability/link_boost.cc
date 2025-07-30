#include "../ability/link_boost.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/payload.h"
#include "../utils/position.h"
#include <map>
#include <sstream>
#include <string>

using namespace std;

LinkBoost::LinkBoost(Permission &permission, GameState &gameState)
    : Ability("L", permission, gameState) {}

void LinkBoost::execute(const Payload &payload) {
  // Input: string (single char) representing a link
  // Example: "a"
  string args = payload.get("args");
  stringstream ss(args);
  string linkIdStr;

  ss >> linkIdStr;
  if (linkIdStr.length() != 1) {
    return;
  }
  char linkId = linkIdStr[0];

  Player &currentPlayer = gameState.getCurPlayer();
  shared_ptr<Link> targetLink = gameState.getLink(linkId);

  // A player can only boost their own link
  if (!targetLink ||
      targetLink->permission.getOwner().get() != &currentPlayer) {
    return;
  }

  // Apply boost
  map<string, Position> currentMoves = targetLink->getMoves();
  map<string, Position> boostedMoves;

  for (const auto &move : currentMoves) {
    Position originalDelta = move.second;
    pair<int, int> deltaCoords = originalDelta.getPosition();

    int newRow = deltaCoords.first;
    int newCol = deltaCoords.second;

    if (newRow > 0) {
      newRow++;
    } else if (newRow < 0) {
      newRow--;
    }

    if (newCol > 0) {
      newCol++;
    } else if (newCol < 0) {
      newCol--;
    }

    boostedMoves[move.first] = Position{newRow, newCol};
  }
  targetLink->setMoves(boostedMoves);

  notifyAbilityUsed();
}
