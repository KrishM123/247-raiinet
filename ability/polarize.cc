#include "../ability/polarize.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/payload.h"
#include "../utils/event_types.h"
#include "../utils/message_queue.h"
#include "../utils/game_event.h"
#include <sstream>
#include <string>

using namespace std;

Polarize::Polarize(Permission &permission, GameState &gameState)
    : Ability("P", permission, gameState) {}

Polarize::~Polarize() {}

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

  if (!targetLink || targetLink->permission.getOwner().get() != &currentPlayer) {
    return;
  }

  // Apply polarity change
  int currentType = targetLink->getType();
  if (currentType == 0) {
    targetLink->setType(1); // Change Data to Virus
  } else if (currentType == 1) {
    targetLink->setType(0); // Change Virus to Data
  }

  std::shared_ptr<MessageQueue> queue = MessageQueue::getInstance();

  map<string, string> payloadMap;
  payloadMap["x"] = to_string(targetLink->getPosition().getPosition().first);
  payloadMap["y"] = to_string(targetLink->getPosition().getPosition().second);
  EventType eventType = EventType::Polarize;
  Payload eventPayload{payloadMap};
  queue->enqueueEvent(GameEvent(eventType, eventPayload));

  notifyAbilityUsed();
}