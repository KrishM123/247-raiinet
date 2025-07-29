#include "graphics_display.h"
#include "../game/cell.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include <iostream>
#include <string>

GraphicsDisplay::GraphicsDisplay(GameState &gameState, int playerView)
    : View(gameState, playerView), window{420, 800},
      gridSize{gameState.getBoard().getGridSize()}, cellSize{50} {

  window.fillRectangle(0, 0, 420, 800, Xwindow::Seashell1);

  auto players = gameState.getPlayers();
  auto player1 = players[0];
  auto player2 = players[1];

  // Player 1 info
  int x = 15;
  int y = 50;
  window.drawString(x, y, "Player 1:");
  y += 25;
  window.drawString(x, y, "Downloaded: 0D 0V");
  y += 25;
  window.drawString(x, y, "Abilities: 5");
  y += 25;

  auto p1Links = player1->getLinks();
  for (int i = 0; i < 4; ++i) {
    window.drawString(x + i * 100, y, p1Links[i]->getDetails());
  }
  y += 25;
  for (int i = 4; i < 8; ++i) {
    window.drawString(x + (i - 4) * 100, y, p1Links[i]->getDetails());
  }

  // Draw board
  int boardY = y + 25;
  for (int i = 1; i <= gridSize; ++i) {
    for (int j = 1; j <= gridSize; ++j) {
      int cellX = (j - 1) * cellSize + 10;
      int cellY = boardY + (i - 1) * cellSize;
      int color = Xwindow::White;
      Cell &cell = gameState.getBoard().getCell(Position{i, j});
      if (cell.getType() == 1) { // P1 server
        color = Xwindow::Gray30;
      }

      // auto occupants = cell.getOccupants();
      // bool p1HasLink = false;
      // for (const auto &occupant : occupants) {
      //   auto link = std::dynamic_pointer_cast<Link>(occupant);
      //   if (link && link->permission.getOwner()->getPlayerNumber() == 1) {
      //     p1HasLink = true;
      //     break;
      //   }
      // }

      // if (p1HasLink) {
      //   color = Xwindow::Seagreen2;
      //   if (cell.getType() == 2) { // P2 server
      //     color = Xwindow::Brown1;
      //   }
      // }

      window.fillRectangle(cellX, cellY, cellSize, cellSize, Xwindow::Black);
      window.fillRectangle(cellX + 1, cellY + 1, cellSize - 2, cellSize - 2,
                           color);

      // for (const auto &occupant : occupants) {
      //   auto link = std::dynamic_pointer_cast<Link>(occupant);
      //   if (link) {
      //     char linkName = link->getName();
      //     window.drawString(cellX + cellSize / 2, cellY + cellSize / 2,
      //                       std::string(1, linkName));
      //   }
      // }
    }
  }

  // Player 2 info
  y = boardY + (gridSize + 1) * cellSize;
  window.drawString(x, y, "Player 2:");
  y += 25;
  window.drawString(x, y, "Downloaded: 0D 0V");
  y += 25;
  window.drawString(x, y, "Abilities: 5");
  y += 25;
  auto p2Links = player2->getLinks();
  for (int i = 0; i < 4; ++i) {
    window.drawString(x + i * 100, y, p2Links[i]->getDetails());
  }
  y += 25;
  for (int i = 4; i < 8; ++i) {
    window.drawString(x + (i - 4) * 100, y, p2Links[i]->getDetails());
  }
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