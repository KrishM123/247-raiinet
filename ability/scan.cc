#include "../ability/scan.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/payload.h"
#include "../utils/permission.h"
#include <sstream>

using namespace std;

Scan::Scan(Permission &permission, GameState &gameState)
    : Ability("A", permission, gameState) {}

void Scan::execute(const Payload &payload) {
  // Input: string (single char) representing a link. Example: "A"
  string args = payload.get("args");
  stringstream ss(args);
  string linkIdStr;
  ss >> linkIdStr;

  if (linkIdStr.length() != 1) {
    return;
  }
  char linkId = linkIdStr[0];

  // Find the target link
  shared_ptr<Link> targetLink = gameState.getLink(linkId);

  if (!targetLink) {
    return;
  }

  // Get current player's shared_ptr
  Player &currentPlayer = gameState.getCurPlayer();
  shared_ptr<Player> currentPlayerPtr =
      gameState.getPlayers()[currentPlayer.getPlayerNumber() - 1];

  // Add the current player to the link's visibility list
  if (currentPlayerPtr) {
    targetLink->permission.addViewer(currentPlayerPtr);
  }
  
  notifyAbilityUsed();
}
