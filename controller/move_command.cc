#include "move_command.h"

#include <iostream>
#include <sstream>

#include "../game/game_state.h"
#include "../game/player.h"
#include "../utils/payload.h"

MoveCommand::MoveCommand(GameState &gameState, Payload &payload)
    : Command(gameState), command(payload.get("command")) {}

MoveCommand::~MoveCommand() {
  // Virtual destructor implementation
}

void MoveCommand::execute() {
  std::stringstream ss(command);
  std::string link;
  ss >> link;
  std::string direction;
  ss >> direction;

  std::shared_ptr<Link> link_ptr = gameState.getLink(link[0]);
  if (gameState.getCurPlayer().getPlayerNumber() != link_ptr->permission.getOwner()->getPlayerNumber()) {
    throw std::invalid_argument("Cannot move link that doesn't belong to the current player");
  }
  gameState.moveLink(link_ptr, direction);
  gameState.nextTurn();
}