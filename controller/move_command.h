#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include <string>
#include "command.h"

class MoveCommand final : public Command {
public:
    MoveCommand(std::string command);
    ~MoveCommand();
    void execute() override;
};

#endif // MOVE_COMMAND_H