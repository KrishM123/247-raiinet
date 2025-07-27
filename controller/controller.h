#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include "command.h"
#include "../game/game_state.h"

class Command;
class View;

class Controller final {
    std::vector<View> views;
    GameState gameState;
    std::vector<std::string> linkFiles;
    std::vector<std::string> abilities;
    bool graphicsEnabled;
    
    std::unique_ptr<Command> parseInput(const std::string& input);
    void executeCommand(std::unique_ptr<Command> command);
    void parseCommandLineArgs(int argc, char* argv[]);
    void loadLinksFromFile(const std::string& filename, std::shared_ptr<Player> player);

public:
    Controller();
    ~Controller();
    void init(int argc = 0, char* argv[] = nullptr);
    void play();
};

#endif // CONTROLLER_H