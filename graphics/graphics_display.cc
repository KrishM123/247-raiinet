#include "graphics_display.h"
#include "../game/game_state.h"
#include <iostream>

GraphicsDisplay::GraphicsDisplay(GameState &gameState, int playerView)
    : View(gameState, playerView) {
  // Graphics display initialization
}

GraphicsDisplay::~GraphicsDisplay() {
  // Graphics display cleanup
}

void GraphicsDisplay::printGame() {
  // Graphics game printing implementation
  std::cout << "Graphics display not fully implemented" << std::endl;
}

void GraphicsDisplay::printAbilities() {
  // Graphics abilities printing implementation
  std::cout << "Graphics abilities display not fully implemented" << std::endl;
}