#include "../ability/polarize.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/payload.h"
#include <sstream>
#include <string>

using namespace std;

Polarize::Polarize(Permission &permission, GameState &gameState)
    : Ability("P", permission, gameState) {}

void Polarize::execute(const Payload &payload) {
  // Input: string (single char) representing a link
  // Example: "d"

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

  if (!targetLink ||
      targetLink->permission.getOwner().get() != &currentPlayer) {
    return;
  }

  // Apply polarity change
  int currentType = targetLink->getType();
  if (currentType == 0) {
    targetLink->setType(1); // Change Data to Virus
  } else if (currentType == 1) {
    targetLink->setType(0); // Change Virus to Data
  }

  notifyPolarize(targetLink->getPosition());

  notifyAbilityUsed();
}
