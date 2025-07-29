#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "command.h"
#include <string>

class Payload;

class MoveCommand final : public Command {
  std::string command;

public:
  MoveCommand(GameState &gameState, Payload &payload);
  ~MoveCommand();
  void execute() override;
};

#endif // MOVE_COMMAND_H