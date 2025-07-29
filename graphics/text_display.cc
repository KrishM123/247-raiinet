#include "text_display.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include <iostream>

TextDisplay::TextDisplay(GameState &gameState, int playerView)
    : View(gameState, playerView) {}

TextDisplay::~TextDisplay() {
  // Virtual destructor implementation
}

void TextDisplay::printGame() {
  printPlayer(0);
  printBoard();
  printPlayer(1);
}

void TextDisplay::printBoard() {
  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {
      std::cout << (linksOnBoard[i][j] != "" ? linksOnBoard[i][j]
                                             : board[i][j]);
    }
    std::cout << std::endl;
  }
}

void TextDisplay::printPlayer(int player) {
  std::cout << "Player " << player + 1 << ":" << std::endl;
  std::cout << "Downloaded: " << downloadedData[player] << "D, "
            << downloadedVirus[player] << "V" << std::endl;
  std::cout << "Abilities: " << unusedAbilities << std::endl;
  Player &playerDisplay = *gameState.getPlayers()[playerView];
  Player &curPlayer = *gameState.getPlayers()[player];
  for (int i = 0; i < curPlayer.getLinks().size() / 2; i++) {
    Link &link = *curPlayer.getLinks()[i];
    std::cout << link.getName() << ": "
              << (link.permission.viewableBy(playerDisplay)
                      ? link.getDetails() + " "
                      : "? ");
  }
  std::cout << std::endl;
  for (int i = 4; i < curPlayer.getLinks().size(); i++) {
    Link &link = *curPlayer.getLinks()[i];
    std::cout << link.getName() << ": "
              << (link.permission.viewableBy(playerDisplay)
                      ? link.getDetails() + " "
                      : "? ");
  }
  std::cout << std::endl;
}
