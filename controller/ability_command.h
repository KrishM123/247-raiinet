#ifndef ABILITY_COMMAND_H
#define ABILITY_COMMAND_H

#include <string>
#include "command.h"

class AbilityCommand final : public Command {
public:
    AbilityCommand(std::string command);
    ~AbilityCommand();
    void execute() override;
};

#endif // ABILITY_COMMAND_H