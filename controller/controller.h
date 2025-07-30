#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../game/game_state.h"
#include "../utils/message_subscriber.h"
#include "command.h"
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

class Command;
class View;

class Controller final : public MessageSubscriber {
  bool playing;
  bool graphicsEnabled;
  bool obstaclesEnabled;
  bool sideMovesEnabled;
  int boardSize;
  int numPlayers;
  std::vector<std::string> linkFiles;
  std::vector<std::string> abilities;
  std::vector<std::unique_ptr<View>> views;
  std::unique_ptr<GameState> gameState;

  std::unique_ptr<Command> parseInput(const std::string &input);
  void executeCommand(std::unique_ptr<Command> command);
  void parseCommandLineArgs(int argc, char *argv[]);
  std::vector<std::string> loadLinkFiles();
  void play(std::string filename);

public:
  Controller(int numPlayers, int boardSize);
  ~Controller();
  void init(int argc = 0, char *argv[] = nullptr);
  void play();

  void notify(GameEvent &event) override;
};

#endif // CONTROLLER_H
