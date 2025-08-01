#ifndef GRAPHICS_DISPLAY_H
#define GRAPHICS_DISPLAY_H

#include "view.h"
#include "x_window.h"

class GameState;

class GraphicsDisplay final : public View {
public:
  GraphicsDisplay(GameState &gameState, int playerView);

  void printGame() override;

private:
  void updatePlayerInfo();
  Xwindow window;
  int gridSize;
  int cellSize;
  void drawCell(int x, int y, char item);
};

#endif // GRAPHICS_DISPLAY_H
