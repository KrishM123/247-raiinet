#include "../ability/ability.h"
#include "../game/game_state.h"
#include "../utils/event_types.h"
#include "../utils/payload.h"
#include "../utils/position.h"
#include "../utils/message_queue.h"
#include "../utils/game_event.h"

using namespace std;

Ability::Ability(string name, Permission &permission, GameState &gameState)
    : name{name}, permission{permission}, gameState{gameState} {}

Ability::~Ability() {}

void Ability::notifyAbilityUsed() {
  // Set the ability to used
  this->used = true;
  this->permission.setVisibleTo(gameState.getPlayers());
}

void Ability::notifyAbilityPlaced(Position targetPos, string marker) {
  // Create a payload for the event
  map<string, string> payloadMap;
  payloadMap["x"] = to_string(targetPos.getPosition().first);
  payloadMap["y"] = to_string(targetPos.getPosition().second);
  // Add the marker to the board
  payloadMap["marker"] = marker;
  EventType eventType = EventType::AbilityPlaced;
  // Create a payload for the event
  Payload eventPayload{payloadMap};
  // Enqueue the event
  MessageQueue::getInstance()->enqueueEvent(GameEvent(eventType, eventPayload));
}

void Ability::notifyPolarize(Position targetPos) {
  // Create a payload for the event
  map<string, string> payloadMap;
  payloadMap["x"] = to_string(targetPos.getPosition().first);
  payloadMap["y"] = to_string(targetPos.getPosition().second);
  EventType eventType = EventType::Polarize;
  Payload eventPayload{payloadMap};
  MessageQueue::getInstance()->enqueueEvent(GameEvent(eventType, eventPayload));
}

void Ability::notifyOccupantRemoved(Position pos) {
  // Create a payload for the event
  map<string, string> payloadMap;
  payloadMap["x"] = to_string(pos.getPosition().first);
  payloadMap["y"] = to_string(pos.getPosition().second);
  EventType eventType = EventType::OccupantRemoved;
  Payload eventPayload{payloadMap};
  MessageQueue::getInstance()->enqueueEvent(GameEvent(eventType, eventPayload));
}

void Ability::notifyOccupantAdded(Position pos, char occupant) {
  // Create a payload for the event
  map<string, string> payloadMap;
  payloadMap["x"] = to_string(pos.getPosition().first);
  payloadMap["y"] = to_string(pos.getPosition().second);
  payloadMap["occupant"] = occupant;
  EventType eventType = EventType::OccupantAdded;
  Payload eventPayload{payloadMap};
  MessageQueue::getInstance()->enqueueEvent(GameEvent(eventType, eventPayload));
}
