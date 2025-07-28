#ifndef ABILITY_COMMAND_H
#define ABILITY_COMMAND_H

#include <string>
#include "command.h"

class Payload;

class AbilityCommand final : public Command {
    std::string command;
public:
    AbilityCommand(GameState& gameState, Payload& payload);
    ~AbilityCommand();
    void execute() override;
};

#endif // ABILITY_COMMAND_H