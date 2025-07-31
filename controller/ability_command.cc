#include "ability_command.h"
#include "../ability/ability.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../utils/event_types.h"
#include "../utils/message_queue.h"
#include "../utils/payload.h"
#include <map>
#include <stdexcept>

AbilityCommand::AbilityCommand(GameState &gameState, Payload &payload)
    : Command(gameState), command(payload.get("command")) {}

void AbilityCommand::execute() {
  std::string ability = command.substr(0, 1);
  // If not found, ability used is invalid
  bool found = false;
  for (auto &ability_ptr : gameState.getCurPlayer().getAbilities()) {
    if (ability_ptr->name == ability && !ability_ptr->used && !found) {
      found = true;
      // Only try to get arguments if the command is longer than just the
      // ability name
      std::string args = "";
      if (command.length() > 1) {
        args = command.substr(2);
      }
      // Execute the ability with the arguments
      std::map<std::string, std::string> payloadMap = {{"args", args}};
      try {
        ability_ptr->execute(Payload(payloadMap));
      } catch (const std::exception &e) {
        throw std::invalid_argument("Error executing ability: " + ability);
      }
      gameState.abilityUsed = true;
    }
  }
  if (!found) {
    throw std::invalid_argument("Ability not found: " + ability);
  }
}
