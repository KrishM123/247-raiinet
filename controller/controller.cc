#include "controller.h"
#include "../game/player.h"
#include "../graphics/graphics_display.h"
#include "../graphics/text_display.h"
#include "../graphics/view.h"
#include "../utils/payload.h"
#include "ability_command.h"
#include "move_command.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

Controller::Controller(int numPlayers, int boardSize)
    : graphicsEnabled{false}, boardSize{boardSize}, numPlayers{numPlayers},
      views{}, linkFiles{std::vector<std::string>(numPlayers)},
      abilities{std::vector<std::string>(numPlayers)} {}

Controller::~Controller() {
  // Virtual destructor implementation
}

void Controller::init(int argc, char *argv[]) {
  if (argc > 0 && argv != nullptr) {
    parseCommandLineArgs(argc, argv);
  }

  gameState = std::make_unique<GameState>(numPlayers, boardSize,
                                          loadLinkFiles(), abilities);
  for (int i = 0; i < numPlayers; i++) {
    if (!graphicsEnabled) {
      views.push_back(std::make_unique<TextDisplay>(*gameState, i));
    } else {
      views.push_back(std::make_unique<GraphicsDisplay>(*gameState, i));
    }
  }
  gameState->init();
}

void Controller::parseCommandLineArgs(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg.find("link") != std::string::npos) {
      linkFiles[arg[arg.size() - 1] - '1'] = arg;
    } else if (arg.find("ability") != std::string::npos) {
      abilities[arg[arg.size() - 1] - '1'] = arg;
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

void Controller::play() {
  std::cout << "Game started" << std::endl;
  while (!gameState->isWon()) {
    std::string input;
    std::cin >> input;
    std::unique_ptr<Command> command = parseInput(input);
    executeCommand(std::move(command));
  }
  std::cout << "Game over" << std::endl;
}

void Controller::play(std::string filename) {
  std::ifstream file(filename);
  std::string line;
  while (std::getline(file, line) && !gameState->isWon()) {
    std::unique_ptr<Command> command = parseInput(line);
    if (command != nullptr) {
      executeCommand(std::move(command));
    }
  }
}

std::unique_ptr<Command> Controller::parseInput(const std::string &input) {
  std::stringstream ss(input);
  std::string command;
  ss >> command;
  if (command.find("move") != std::string::npos) {
    Payload payload(
        std::map<std::string, std::string>{{"command", command.substr(6)}});
    abilityUsed = false;
    gameState->nextTurn();
    return std::make_unique<MoveCommand>(*gameState, payload);
  } else if (command.find("ability") != std::string::npos) {
    if (!abilityUsed) {
      Payload payload(
          std::map<std::string, std::string>{{"command", command.substr(8)}});
      abilityUsed = true;
      return std::make_unique<AbilityCommand>(*gameState, payload);
    }
  } else if (command.find("board") != std::string::npos) {
    for (int i = 0; i < numPlayers; i++) {
      if (gameState->getPlayers().at(i)->getPlayerNumber() ==
          gameState->getCurPlayer().getPlayerNumber()) {
        views[i]->printGame();
        break;
      }
    }
  } else if (command.find("abilities") != std::string::npos) {
    for (int i = 0; i < numPlayers; i++) {
      if (gameState->getPlayers().at(i)->getPlayerNumber() ==
          gameState->getCurPlayer().getPlayerNumber()) {
        views[i]->printAbilities();
        break;
      }
    }
  } else if (command.find("sequence") != std::string::npos) {
    play(command.substr(9));
  } else if (command.find("quit") != std::string::npos) {
    gameState->endGame();
  }
  return nullptr;
}

void Controller::executeCommand(std::unique_ptr<Command> command) {
  command->execute();
}