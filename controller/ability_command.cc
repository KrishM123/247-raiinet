#include <map>
#include <stdexcept>
#include "ability_command.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../ability/ability.h"
#include "../utils/payload.h"

AbilityCommand::AbilityCommand(GameState& gameState, Payload& payload) : Command(gameState), command(payload.get("command")) {}

void AbilityCommand::execute() {
    std::string ability = command.substr(7, 1);
    bool found = false;
    for (auto& ability_ptr : gameState.getCurPlayer().getAbilities()) {
        if (ability_ptr->getName() == ability) {
            found = true;
            std::map<std::string, std::string> payload = {{"arguments", command.substr(9)}};
            ability_ptr->execute(Payload(payload));
        }
    }
    if (!found) {
        throw std::invalid_argument("Ability not found: " + ability);
    }
}