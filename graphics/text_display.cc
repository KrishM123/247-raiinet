#include "text_display.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include <iostream>

TextDisplay::TextDisplay(GameState &gameState, int playerView)
    : View(gameState, playerView) {}

void TextDisplay::printGame() {
  printPlayer(0);
  std::cout << "========" << std::endl;
  printBoard();
  std::cout << "========" << std::endl;
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

  int ununsedAbilityCount = 0;
  std::vector<std::shared_ptr<Ability>> abilities =
      gameState.getPlayers()[player]->getAbilities();
  for (int i = 0; i < abilities.size(); ++i) {
    if (!abilities[i]->used) {
      ununsedAbilityCount++;
    }
  }
  std::cout << "Abilities: " << ununsedAbilityCount << std::endl;

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
