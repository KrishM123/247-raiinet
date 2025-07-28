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
    bool graphicsEnabled;
    bool abilityUsed;
    int boardSize;
    int numPlayers;
    std::vector<std::string> linkFiles;
    std::vector<std::string> abilities;
    std::vector<std::unique_ptr<View>> views;
    GameState gameState;
    
    std::unique_ptr<Command> parseInput(const std::string& input);
    void executeCommand(std::unique_ptr<Command> command);
    void parseCommandLineArgs(int argc, char* argv[]);
    std::vector<std::string> loadLinkFiles();
    void play(std::string filename);

public:
    Controller(int numPlayers, int boardSize);
    ~Controller();
    void init(int argc = 0, char* argv[] = nullptr);
    void play();
};

#endif // CONTROLLER_H