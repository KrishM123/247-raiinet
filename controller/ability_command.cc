#include "ability_command.h"
#include "../ability/ability.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../controller/event_types.h"
#include "../utils/message_queue.h"
#include "../utils/payload.h"
#include <map>
#include <stdexcept>

AbilityCommand::AbilityCommand(GameState &gameState, Payload &payload)
    : Command(gameState), command(payload.get("command")) {}

AbilityCommand::~AbilityCommand() {
  // Virtual destructor implementation
}

void AbilityCommand::execute() {
  std::string ability = command.substr(0, 1);
  bool found = false;
  for (auto &ability_ptr : gameState.getCurPlayer().getAbilities()) {
    if (ability_ptr->name == ability && !ability_ptr->used && !found) {
      found = true;
      std::map<std::string, std::string> payload = {
          {"args", command.substr(2)}};
        ability_ptr->execute(Payload(payload));
      std::map<std::string, std::string> eventPayloadMap = {
          {"player", std::to_string(gameState.getCurPlayer().getPlayerNumber() - 1)},
          {"ability", ability}};
      Payload eventPayload(eventPayloadMap);
      EventType eventType = EventType::AbilityUsed;
      MessageQueue::getInstance()->enqueueEvent(GameEvent(eventType, eventPayload));
    }
  }
  if (!found) {
    throw std::invalid_argument("Ability not found: " + ability);
  }
}