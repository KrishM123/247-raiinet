#include "../ability/swaplinks.h"
#include "../game/board.h"
#include "../game/cell.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/message_queue.h"
#include "../utils/payload.h"
#include "trigger.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

Swaplinks::Swaplinks(Permission &permission, GameState &gameState)
    : Ability("S", permission, gameState) {}

Swaplinks::~Swaplinks() {}

void Swaplinks::execute(const Payload &payload) {
  // Input:
  // Example: "a d"
  string args = payload.get("args");
  stringstream ss(args);
  string linkId1Str, linkId2Str;
  ss >> linkId1Str >> linkId2Str;

  if (linkId1Str.length() != 1 || linkId2Str.length() != 1 || !ss.eof())
    return;

  Player &currentPlayer = gameState.getCurPlayer();
  shared_ptr<Link> link1 = gameState.getLink(linkId1Str[0]);
  shared_ptr<Link> link2 = gameState.getLink(linkId2Str[0]);
  if (link1->permission.getOwner()->getPlayerNumber() !=
          currentPlayer.getPlayerNumber() ||
      link2->permission.getOwner()->getPlayerNumber() !=
          currentPlayer.getPlayerNumber())
    throw std::invalid_argument("Cannot swap links that do not belong to the current player");

  if (link1->getIsDownloaded() || link2->getIsDownloaded())
    throw std::invalid_argument("Cannot swap a downloaded link.");

  if (!link1 || !link2 || link1 == link2)
    return;

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

  std::shared_ptr<MessageQueue> queue = MessageQueue::getInstance();

  map<string, string> payloadMap;
  payloadMap["x"] = to_string(pos1.getPosition().first);
  payloadMap["y"] = to_string(pos1.getPosition().second);
  EventType eventType = EventType::OccupantRemoved;
  Payload eventPayload{payloadMap};
  queue->enqueueEvent(GameEvent(eventType, eventPayload));

  map<string, string> payloadMap2;
  payloadMap2["x"] = to_string(pos2.getPosition().first);
  payloadMap2["y"] = to_string(pos2.getPosition().second);
  EventType eventType2 = EventType::OccupantRemoved;
  Payload eventPayload2{payloadMap2};
  queue->enqueueEvent(GameEvent(eventType, eventPayload));

  map<string, string> payloadMap3;
  payloadMap3["x"] = to_string(pos1.getPosition().first);
  payloadMap3["y"] = to_string(pos1.getPosition().second);
  payloadMap3["occupant"] = link2->getName();
  EventType eventType3 = EventType::OccupantAdded;
  Payload eventPayload3{payloadMap3};
  queue->enqueueEvent(GameEvent(eventType3, eventPayload3));

  map<string, string> payloadMap4;
  payloadMap4["x"] = to_string(pos2.getPosition().first);
  payloadMap4["y"] = to_string(pos2.getPosition().second);
  payloadMap4["occupant"] = link1->getName();
  EventType eventType4 = EventType::OccupantAdded;
  Payload eventPayload4{payloadMap4};
  queue->enqueueEvent(GameEvent(eventType4, eventPayload4));

  notifyAbilityUsed();
}