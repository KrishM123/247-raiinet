#ifndef TRIGGER_H
#define TRIGGER_H

#include "../game/occupant.h"
#include <functional>

class Link;
class Position;
class Permission;
class GameState;

class Trigger : public Occupant {
protected:
  GameState &gameState;
  std::function<void()> triggerAction;
  
public:
  Trigger(GameState &gameState, const Position &pos, const Permission &perm);
  Trigger(GameState &gameState, const Position &pos, const Permission &perm,
          std::function<void()> action);

  void trigger();
};

#endif // TRIGGER_H
