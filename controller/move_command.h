#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "command.h"

class MoveCommand final : public Command {
public:
    MoveCommand();
    ~MoveCommand();
    void execute() override;
};

#endif // MOVE_COMMAND_H