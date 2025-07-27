#include <sstream>
#include "move_command.h"
#include "../game/game_state.h"
#include "../game/player.h"

MoveCommand::MoveCommand(std::string command, GameState& gameState) : Command(gameState), command(command) {}

void MoveCommand::execute() {
    std::stringstream ss(command);
    std::string link;
    ss >> link;
    std::string direction;
    ss >> direction;

    std::shared_ptr<Link> link_ptr = gameState.getLinks().at(gameState.getCurPlayer().linksMap.at(link[0]));

    gameState.moveLink(link_ptr, direction);    
}