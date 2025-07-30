#include "../ability/swaplinks.h"
#include "../game/board.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/message_queue.h"
#include "../utils/payload.h"
#include "trigger.h"
#include <sstream>
#include <string>

using namespace std;

Swaplinks::Swaplinks(Permission &permission, GameState &gameState)
    : Ability("S", permission, gameState) {}

void Swaplinks::execute(const Payload &payload) {
  // Input:
  // Example: "a d"
  string args = payload.get("args");
  stringstream ss(args);
  string linkId1Str, linkId2Str;
  ss >> linkId1Str >> linkId2Str;

  if (linkId1Str.length() != 1 || linkId2Str.length() != 1)
    return;

  Player &currentPlayer = gameState.getCurPlayer();
  shared_ptr<Link> link1 = gameState.getLink(linkId1Str[0]);
  shared_ptr<Link> link2 = gameState.getLink(linkId2Str[0]);
  if (link1->permission.getOwner()->getPlayerNumber() !=
          currentPlayer.getPlayerNumber() ||
      link2->permission.getOwner()->getPlayerNumber() !=
          currentPlayer.getPlayerNumber()) {
    throw std::invalid_argument(
        "Cannot swap links that do not belong to the current player");
  }

  if (link1->getIsDownloaded() || link2->getIsDownloaded()) {
    throw std::invalid_argument("Cannot swap a downloaded link.");
  }

  if (!link1 || !link2 || link1 == link2) {
    throw std::invalid_argument("Invalid link IDs");
  }

  Position pos1 = link1->getPosition();
  Position pos2 = link2->getPosition();
  Board &board = gameState.getBoard();

  // Swap
  // Remove both links from their current positions
  board.removeOccupant(link1, pos1);
  board.removeOccupant(link2, pos2);

  // Place both links in their new positions
  board.placeOccupant(link1, pos2);
  board.placeOccupant(link2, pos1);

  // Update internal position trackers of links
  link1->setPosition(pos2);
  link2->setPosition(pos1);

  notifyOccupantRemoved(pos1);
  notifyOccupantRemoved(pos2);
  notifyOccupantAdded(pos1, link2->getName());
  notifyOccupantAdded(pos2, link1->getName());

  notifyAbilityUsed();
}
