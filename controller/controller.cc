#include "controller.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../game/player.h"
#include "../graphics/graphics_display.h"
#include "../graphics/text_display.h"
#include "../graphics/view.h"
#include "../utils/payload.h"
#include "ability_command.h"
#include "move_command.h"

Controller::Controller(int numPlayers, int boardSize)
    : graphicsEnabled{false}, obstaclesEnabled{false}, sideMovesEnabled{false},
      boardSize{boardSize}, numPlayers{numPlayers}, views{},
      linkFiles{std::vector<std::string>(numPlayers)},
      abilities{std::vector<std::string>(numPlayers)}, playing{true} {}

Controller::~Controller() {
  // Unsubscribe from message queue and stop it
  MessageQueue::getInstance()->unsubscribe(this);
  MessageQueue::getInstance()->stop();
}

void Controller::init(int argc, char *argv[]) {
  if (argc > 0 && argv != nullptr) {
    parseCommandLineArgs(argc, argv);
  }
  // Initialize game state
  gameState = std::make_unique<GameState>(numPlayers, boardSize,
                                          loadLinkFiles(), abilities,
                                          obstaclesEnabled, sideMovesEnabled);
  gameState->init();
  // Initialize views
  for (int i = 0; i < numPlayers; i++) {
    if (graphicsEnabled) {
      views.push_back(std::make_unique<GraphicsDisplay>(*gameState, i));
    } else {
      views.push_back(std::make_unique<TextDisplay>(*gameState, i));
    }
  }

  // Start message queue and subscribe to it
  MessageQueue::getInstance()->start();
  MessageQueue::getInstance()->subscribe(this);
}

void Controller::parseCommandLineArgs(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    // Parse command line arguments
    if (arg.find("link") != std::string::npos) {
      linkFiles[arg[arg.size() - 1] - '1'] = argv[++i];
    } else if (arg.find("ability") != std::string::npos) {
      abilities[arg[arg.size() - 1] - '1'] = argv[++i];
    } else if (arg == "-graphics") {
      graphicsEnabled = true;
    } else if (arg == "-obstacles") {
      obstaclesEnabled = true;
    } else if (arg == "-sidemoves") {
      sideMovesEnabled = true;
    }
  }
}

std::vector<std::string> Controller::loadLinkFiles() {
  // Load link files
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
  // While game is not won and playing
  while (!gameState->isWon() && playing) {
    // Get input
    std::string input;
    std::getline(std::cin, input);
    // Parse input
    std::unique_ptr<Command> command = parseInput(input);
    // If command is not null, execute it
    if (command != nullptr) {
      executeCommand(std::move(command));
    }
  }
  // Output example: Game over
  std::cout << "Game over" << std::endl;
  // If game is still playing, output winner
  if (playing) {
    std::shared_ptr<Player> winner = gameState->getWinner();
    std::cout << "Winner: Player " << winner->getPlayerNumber() << std::endl;
  }
}

void Controller::play(std::string filename) {
  // Open file
  std::ifstream file(filename);
  std::string line;
  // While file has lines and game is not won and playing
  while (std::getline(file, line) && !gameState->isWon() && playing) {
    std::unique_ptr<Command> command = parseInput(line);
    if (command != nullptr) {
      executeCommand(std::move(command));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}

std::unique_ptr<Command> Controller::parseInput(const std::string &input) {
  try {
    // If input is move, create move command
    if (input.find("move") != std::string::npos) {
      Payload payload(
          std::map<std::string, std::string>{{"command", input.substr(5)}});
      return std::make_unique<MoveCommand>(*gameState, payload);
    } else if (input.find("ability") != std::string::npos) {
      // If ability already used, throw error
      if (!(gameState->abilityUsed)) {
        Payload payload(
            std::map<std::string, std::string>{{"command", input.substr(8)}});
        return std::make_unique<AbilityCommand>(*gameState, payload);
      } else {
        // If ability not used, create ability command
        throw std::invalid_argument("Ability already used");
      }
      // If input is board, print game
    } else if (input == "board") {
      // For each player, if player number matches current player, print game
      for (int i = 0; i < numPlayers; i++) {
        if (gameState->getPlayers().at(i)->getPlayerNumber() ==
            gameState->getCurPlayer().getPlayerNumber()) {
          views[i]->printGame();
          break;
        }
      }
      // Print game
      gameState->printGame();
    } else if (input.find("abilities") != std::string::npos) {
      // For each player, if player number matches current player, print
      // abilities
      for (int i = 0; i < numPlayers; i++) {
        if (gameState->getPlayers().at(i)->getPlayerNumber() ==
            gameState->getCurPlayer().getPlayerNumber()) {
          views[i]->printAbilities();
          break;
        }
      }
    } else if (input.find("sequence") != std::string::npos) {
      // If input is sequence, play sequence
      play(input.substr(9));
    } else if (input.find("quit") != std::string::npos) {
      // If input is quit, end game and set playing to false
      gameState->endGame();
      playing = false;
    } else {
      // If input is not valid, throw error
      throw std::invalid_argument("Invalid command");
    }
  } catch (const std::exception &e) {
    std::cerr << "Error parsing command: " << e.what() << std::endl;
    return nullptr;
  } catch (...) {
    std::cerr << "Unknown error occurred while parsing command" << std::endl;
    return nullptr;
  }
}

void Controller::notify(GameEvent &event) {
  // For each view, notify it of the event
  for (auto &view : views) {
    view->notify(event);
    // If graphics enabled, print game
    if (graphicsEnabled) {
      view->printGame();
    }
  }
}

void Controller::executeCommand(std::unique_ptr<Command> command) {
  try {
    // Execute command
    command->execute();
  } catch (const std::exception &e) {
    std::cerr << "Error executing command: " << e.what() << std::endl;
  }
}
