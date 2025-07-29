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
  MessageQueue::getInstance()->unsubscribe(this);
  MessageQueue::getInstance()->stop();
}

void Controller::init(int argc, char *argv[]) {
  if (argc > 0 && argv != nullptr) {
    parseCommandLineArgs(argc, argv);
  }

  gameState = std::make_unique<GameState>(numPlayers, boardSize,
                                          loadLinkFiles(), abilities);
  gameState->init();
  for (int i = 0; i < numPlayers; i++) {
    if (!graphicsEnabled) {
      views.push_back(std::make_unique<TextDisplay>(*gameState, i));
    } else {
      views.push_back(std::make_unique<GraphicsDisplay>(*gameState, i));
    }
  }
  
  MessageQueue::getInstance()->start();
  MessageQueue::getInstance()->subscribe(this);
}

void Controller::parseCommandLineArgs(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg.find("link") != std::string::npos) {
      linkFiles[arg[arg.size() - 1] - '1'] = argv[++i];
    } else if (arg.find("ability") != std::string::npos) {
      abilities[arg[arg.size() - 1] - '1'] = argv[++i];
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
    std::getline(std::cin, input);
    std::unique_ptr<Command> command = parseInput(input);
    if (command != nullptr) {
      executeCommand(std::move(command));
    }
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
  if (input.find("move") != std::string::npos) {
    Payload payload(std::map<std::string, std::string>{{"command", input.substr(5)}});
    abilityUsed = false;
    gameState->nextTurn();
    return std::make_unique<MoveCommand>(*gameState, payload);
  } else if (input.find("ability") != std::string::npos) {
    if (!abilityUsed) {
      Payload payload(
          std::map<std::string, std::string>{{"command", input.substr(7)}});
      abilityUsed = true;
      return std::make_unique<AbilityCommand>(*gameState, payload);
    }
  } else if (input.find("board") != std::string::npos) {
    for (int i = 0; i < numPlayers; i++) {
      if (gameState->getPlayers().at(i)->getPlayerNumber() ==
          gameState->getCurPlayer().getPlayerNumber()) {
        views[i]->printGame();
        break;
      }
    }
  } else if (input.find("abilities") != std::string::npos) {
    for (int i = 0; i < numPlayers; i++) {
      if (gameState->getPlayers().at(i)->getPlayerNumber() ==
          gameState->getCurPlayer().getPlayerNumber()) {
        views[i]->printAbilities();
        break;
      }
    }
  } else if (input.find("sequence") != std::string::npos) {
    play(input.substr(9));
  } else if (input.find("quit") != std::string::npos) {
    gameState->endGame();
  }
  return nullptr;
}

void Controller::notify(GameEvent &event) {
  for (auto &view : views) {
    view->notify(event);
  }
}

void Controller::executeCommand(std::unique_ptr<Command> command) {
  command->execute();
}