#ifndef ABILITY_COMMAND_H
#define ABILITY_COMMAND_H

#include "command.h"
#include <string>

class Payload;

class AbilityCommand final : public Command {
  std::string command;
  void notifyAbilityUsed();

public:
  AbilityCommand(GameState &gameState, Payload &payload);
  void execute() override;
};

#endif // ABILITY_COMMAND_H
