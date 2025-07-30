#include "command.h"
#include "../game/game_state.h"

Command::Command(GameState &gameState) : gameState(gameState) {}

Command::~Command() {}
