#include "controller.h"
#include "../game/player.h"
#include "../ability/barbed_wire.h"
#include "../ability/download.h"
#include "../ability/firewall.h"
#include "../ability/link_boost.h"
#include "../ability/polarize.h"
#include "../ability/scan.h"
#include "../ability/swaplinks.h"
#include "../ability/telescope.h"
#include "../utils/permission.h"
#include <sstream>
#include <fstream>
#include <string>
#include <vector>


Controller::Controller(int numPlayers, int boardSize) : gameState{GameState(numPlayers, boardSize)}, views{}, linkFiles{std::vector<std::string>(numPlayers)}, abilities{std::vector<std::string>(numPlayers)}, graphicsEnabled{false}, numPlayers{numPlayers}, boardSize{boardSize} {}

void Controller::init(int argc, char* argv[]) {
    if (argc > 0 && argv != nullptr) {
        parseCommandLineArgs(argc, argv);
    }

    std::vector<std::shared_ptr<Player>> players = gameState.getPlayers();
    
    for (int i = 0; i < players.size(); i++) {
        if (!linkFiles[i].empty()) {
            loadLinksFromFile(linkFiles[i], players[i]);
        }
        if (!abilities[i].empty()) {
            loadAbilities(abilities[i], players[i]);
        }
    }
    
    for (int i = 0; i < players.size(); i++) {
        std::vector<std::shared_ptr<Link>> links = players[i]->getLinks();
        for (int j = 0; j < links.size(); j++) {
            if (i == 0) {
                Position pos(0, rand() % boardSize);
                links[i]->setPosition(pos);
            } else if (i == 1) {
                Position pos(boardSize - 1, rand() % boardSize);
                links[i]->setPosition(pos);
            }
        }
    }
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
    std::vector<std::shared_ptr<Link>> links = player->getLinks();
    std::ifstream file(filename);
    Permission perm(player);
    std::string line;

    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string linkType;
        int i = 0;

        while (iss >> linkType) {
            // Parse link type ("V1", "D4")
            int strength = linkType[1] - '0';
            
            links[i]->setStrength(strength);
            links[i]->setPermission(perm);
            if (linkType[0] == 'V') {
                links[i]->setType(1);
            } else if (linkType[0] == 'D') {
                links[i]->setType(0);
            }
            i++;
        }
    } else {
        int linkType = 0;
        int strength = 1;
        for (int i = 0; i < links.size() / 2; i++) {
            links[i]->setStrength(strength);
            links[i]->setPermission(perm);
            links[i]->setType(linkType);
            strength += 1;
        }
        linkType = 1;
        strength = 1;
        for (int i = links.size() / 2; i < links.size(); i++) {
            links[i]->setStrength(strength);
            links[i]->setPermission(perm);
            links[i]->setType(linkType);
            strength += 1;
        }
    }
    file.close();
}

void Controller::loadAbilities(const std::string& abilities, std::shared_ptr<Player> player) {
    std::vector<std::shared_ptr<Ability>> new_abilities;
    Permission perm(player);
    if (!abilities.empty()) {
        for (char ability : abilities) {
            if (ability == 'B') {
                new_abilities.push_back(std::make_shared<BarbedWire>(perm, gameState));
            } else if (ability == 'D') {
                new_abilities.push_back(std::make_shared<Download>(perm, gameState));
            } else if (ability == 'F') {
                new_abilities.push_back(std::make_shared<Firewall>(perm, gameState));
            } else if (ability == 'L') {
                new_abilities.push_back(std::make_shared<LinkBoost>(perm, gameState));
            } else if (ability == 'P') {
                new_abilities.push_back(std::make_shared<Polarize>(perm, gameState));
            } else if (ability == 'A') {
                new_abilities.push_back(std::make_shared<Scan>(perm, gameState));
            } else if (ability == 'S') {
                new_abilities.push_back(std::make_shared<Swaplinks>(perm, gameState));
            } else if (ability == 'T') {
                new_abilities.push_back(std::make_shared<Telescope>(perm, gameState));
            }
        }
    } else {
        new_abilities.push_back(std::make_shared<BarbedWire>(perm, gameState));
        new_abilities.push_back(std::make_shared<Download>(perm, gameState));
        new_abilities.push_back(std::make_shared<Firewall>(perm, gameState));
        new_abilities.push_back(std::make_shared<LinkBoost>(perm, gameState));
        new_abilities.push_back(std::make_shared<Polarize>(perm, gameState));
    }
    player->setAbilities(new_abilities);
}