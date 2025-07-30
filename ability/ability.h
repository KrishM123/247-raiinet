#ifndef ABILITY_H
#define ABILITY_H

#include "../utils/permission.h"
#include <string>

class GameState;
class Payload;
class Position;

class Ability {
public:
  Permission permission;
  bool used = false;
  std::string name;

  virtual ~Ability();
  virtual void execute(const Payload &payload) = 0;

protected:
  GameState &gameState;
  Ability(std::string name, Permission &permission, GameState &gameState);
  void notifyAbilityUsed();
  void notifyAbilityPlaced(Position targetPos, string marker);
  void notifyPolarize(Position targetPos);
  void notifyOccupantRemoved(Position targetPos);
  void notifyOccupantAdded(Position targetPos, char occupant);
};

#endif // ABILITY_H
