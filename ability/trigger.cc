#include "trigger.h"
#include "../utils/payload.h"

using namespace std;

Trigger::Trigger(GameState &gameState, const Position &pos,
                 const Permission &perm)
    : Occupant{pos, perm}, gameState{gameState} {}

Trigger::Trigger(GameState &gameState, const Position &pos,
                 const Permission &perm, function<void()> action)
    : Occupant{pos, perm}, gameState{gameState}, triggerAction{action} {}

void Trigger::trigger() {
  triggerAction();
}