#ifndef SINGLE_GRAPHICS_H
#define SINGLE_GRAPHICS_H

#include "view.h"
#include "x_window.h"

class GameState;

class SingleGraphics final : public View {
public:
  SingleGraphics(GameState &gameState, int playerView);

  void printGame() override;

private:
  void updatePlayerInfo();
  Xwindow window;
  int gridSize;
  int cellSize;
  void drawCell(int x, int y, char item);
};

#endif // SINGLE_GRAPHICS_H
