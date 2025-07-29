#ifndef COMMAND_H
#define COMMAND_H

class GameState;

class Command {
protected:
  GameState &gameState;

public:
  Command(GameState &gameState);
  virtual ~Command();
  virtual void execute() = 0;
};

#endif // COMMAND_H