#include "controller.h"
#include <fstream>
#include <string>
#include <vector>

Controller::Controller(int numPlayers, int boardSize) : 
    graphicsEnabled{false},
    boardSize{boardSize},
    numPlayers{numPlayers},
    views{}, 
    linkFiles{std::vector<std::string>(numPlayers)}, 
    abilities{std::vector<std::string>(numPlayers)}
    {}

void Controller::init(int argc, char* argv[]) {
    if (argc > 0 && argv != nullptr) {
        parseCommandLineArgs(argc, argv);
    }

    gameState = GameState(numPlayers, boardSize, loadLinkFiles(), abilities);
    
}

void Controller::parseCommandLineArgs(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg.find("link") != std::string::npos) {
            linkFiles[arg[arg.size() - 1] - '0'] = arg;
        } else if (arg.find("ability") != std::string::npos) {
            abilities[arg[arg.size() - 1] - '0'] = arg;
        } else if (arg == "-graphics") {
            graphicsEnabled = true;
        }
    }
}

std::vector<std::string> Controller::loadLinkFiles() {
    std::vector<std::string> links = std::vector<std::string>(numPlayers);
    for (int i = 0; i < numPlayers; i++) {
        std::ifstream file(linkFiles[i]);
        std::string line;
        if (std::getline(file, line)) {
            links[i] = line;
        }
        file.close();
    }
    return links;
}