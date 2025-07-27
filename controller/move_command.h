#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include <string>
#include "command.h"

class MoveCommand final : public Command {
    std::string command;
public:
    MoveCommand(std::string command, GameState& gameState);
    ~MoveCommand();
    void execute() override;
};

#endif // MOVE_COMMAND_H