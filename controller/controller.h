#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include "command.h"
#include "game/game_state.h"

class Command;
class View;

class Controller final {
    std::vector<View> views;
    GameState gameState;
    
    std::unique_ptr<Command> parseInput(const std::string& input);
    void executeCommand(std::unique_ptr<Command> command);

public:
    Controller();
    ~Controller();
    void init();
    void play();
};

#endif // CONTROLLER_H