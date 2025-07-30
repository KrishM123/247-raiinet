#include "move_command.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../utils/payload.h"
#include <sstream>

MoveCommand::MoveCommand(GameState &gameState, Payload &payload)
    : Command(gameState), command(payload.get("command")) {}

void MoveCommand::execute() {
  // Parse command
  std::stringstream ss(command);
  std::string link;
  // Get link
  ss >> link;
  std::string direction;
  // Get direction
  ss >> direction;

  // Get link pointer
  std::shared_ptr<Link> link_ptr = gameState.getLink(link[0]);
  // If link doesn't belong to current player, throw error
  if (gameState.getCurPlayer().getPlayerNumber() !=
      link_ptr->permission.getOwner()->getPlayerNumber()) {
    throw std::invalid_argument(
        "Cannot move link that doesn't belong to the current player");
  }
  // Move link
  gameState.moveLink(link_ptr, direction);
  // Reset ability used
  gameState.abilityUsed = false;
  gameState.nextTurn();
}
