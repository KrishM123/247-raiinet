#ifndef GRAPHICS_DISPLAY_H
#define GRAPHICS_DISPLAY_H

#include "view.h"
#include "x_window.h"

class GameState;

class GraphicsDisplay final : public View {
public:
  GraphicsDisplay(GameState &gameState, int playerView);
  ~GraphicsDisplay() override;

  void printGame() override;
  void printAbilities() override;

private:
  Xwindow window;
  int gridSize;
  int cellSize;
};

#endif // GRAPHICS_DISPLAY_H