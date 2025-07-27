#include "controller.h"
#include "../game/player.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

Controller::Controller() : gameState{GameState(2, 8)}, views{}, linkFiles{std::vector<std::string>(2)}, abilities{std::vector<std::string>(2)}, graphicsEnabled{false} {}

void Controller::init(int argc, char* argv[]) {
    if (argc > 0 && argv != nullptr) {
        parseCommandLineArgs(argc, argv);
    }
    
    Board& board = gameState.getBoard();
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

void Controller::loadLinksFromFile(const std::string& filename, std::shared_ptr<Player> player) {
    std::ifstream file(filename);
    
    std::string line;
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string linkType;
        int i = 0;
        
        while (iss >> linkType) {
            // Parse link type ("V1", "D4")
            int strength = linkType[1] - '0';
            
            player->getLinks()[i]->setStrength(strength);
            if (linkType[0] == 'V') {
                player->getLinks()[i]->setType(1);
            } else if (linkType[0] == 'D') {
                player->getLinks()[i]->setType(0);
            }
            i++;
        }
    }
    
    file.close();
}