#include "../ability/download.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/payload.h"
#include <sstream>

using namespace std;

Download::Download(Permission &permission, GameState &gameState)
    : Ability("D", permission, gameState) {}

void Download::execute(const Payload &payload) {
  // Input: Expects string (single char) representing a link
  // Example: "C"
  string args = payload.get("args");
  stringstream ss(args);
  string linkIdStr;
  ss >> linkIdStr;

  if (linkIdStr.length() != 1) {
    throw std::invalid_argument("Invalid link ID");
  }
  char linkId = linkIdStr[0];

  // Find the target link
  shared_ptr<Link> targetLink = gameState.getLink(linkId);

  // Validate the link and ownership
  Player &currentPlayer = gameState.getCurPlayer();
  if (!targetLink || targetLink->permission.getOwner()->getPlayerNumber() ==
                         currentPlayer.getPlayerNumber()) {
    throw std::invalid_argument(
        "Cannot download link that does not belong to the current player");
  }

  // Get the current player's shared_ptr and download
  shared_ptr<Player> currentPlayerPtr =
      gameState.getPlayers()[currentPlayer.getPlayerNumber() - 1];
  gameState.downloadLink(targetLink, currentPlayerPtr);

  notifyAbilityUsed();
}
