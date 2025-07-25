#ifndef ABILITY_COMMAND_H
#define ABILITY_COMMAND_H

#include "command.h"

class AbilityCommand final : public Command {
public:
    AbilityCommand();
    ~AbilityCommand();
    void execute() override;
};

#endif // ABILITY_COMMAND_H