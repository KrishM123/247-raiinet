#include <iostream>
#include "text_display.h"
#include "../game/game_state.h"
#include "../game/player.h"

TextDisplay::TextDisplay(GameState& gameState) : View(gameState) {}

void TextDisplay::printGame() {
    printPlayer(0);
    printBoard();
    printPlayer(1);
}

void TextDisplay::printBoard() {
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            std::cout << (linksOnBoard[i][j] != "" ? linksOnBoard[i][j] : board[i][j]);
        }
        std::cout << std::endl;
    }
}

void TextDisplay::printPlayer(int player) {
    std::cout << "Player " << player + 1 << ":" << std::endl;
    std::cout << "Downloaded: " << downloadedData[player] << "D, " << downloadedVirus[player] << "V" << std::endl;
    std::cout << "Abilities: " << unusedAbilities[player].size() << std::endl;
    Player& curPlayer = *gameState.getPlayers()[player];
    for (int i = 0; i < curPlayer.getLinks().size() / 2; i++) {
        Link& link = *curPlayer.getLinks()[i];
        std::cout << link.getName() << ": " << (link.permission.viewableBy(curPlayer) ? link.getDetails() : "? ");
    }
    std::cout << std::endl;
    for (int i = 3; i < curPlayer.getLinks().size(); i++) {
        Link& link = *curPlayer.getLinks()[i];
        std::cout << link.getName() << ": " << (link.permission.viewableBy(curPlayer) ? link.getDetails() : "? ");
    }
    std::cout << std::endl;
}