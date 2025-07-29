#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H

#include "view.h"

class GameState;

class TextDisplay final : public View {
  void printPlayer(int player);
  void printBoard();

public:
  TextDisplay(GameState &gameState, int playerView);
  ~TextDisplay() override;

  void printGame() override;
  void printAbilities() override;
};

#endif // TEXT_DISPLAY_H