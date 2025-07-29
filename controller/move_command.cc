#include "move_command.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../utils/payload.h"
#include <iostream>
#include <sstream>

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

  std::cout << "debug: move command started" << std::endl;

  std::shared_ptr<Link> link_ptr = gameState.getLink(link[0]);

  gameState.moveLink(link_ptr, direction);
  std::cout << "debug: move command finished" << std::endl;
}