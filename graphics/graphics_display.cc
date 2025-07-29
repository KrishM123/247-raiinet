#include "graphics_display.h"
#include "../game/cell.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/payload.h"
#include <iostream>
#include <string>

using namespace std;

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
    auto link = p1Links[i];
    std::string details;
    if (playerView == 0) { // P1's view
      details = std::string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      details = std::string(1, link->getName()) + ": ??";
    }
    window.drawString(x + i * 100, y, details);
  }
  y += 25;
  for (int i = 4; i < 8; ++i) {
    auto link = p1Links[i];
    std::string details;
    if (playerView == 0) {
      details = std::string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      details = std::string(1, link->getName()) + ": ??";
    }
    window.drawString(x + (i - 4) * 100, y, details);
  }

  // Draw board
  int boardY = y + 25;
  for (int i = 1; i <= gridSize; ++i) {
    for (int j = 1; j <= gridSize; ++j) {
      int cellX = (j - 1) * cellSize + 10;
      int cellY = boardY + (i - 1) * cellSize;
      int color = Xwindow::White;
      Cell &cell = gameState.getBoard().getCell(Position{i, j});
      if (cell.getType() == 1 || cell.getType() == 2) { // Server port
        color = Xwindow::Gray30;
      }

      if (cell.getOccupants().size() > 1) {
        auto link = std::dynamic_pointer_cast<Link>(cell.getOccupants()[1]);
        if (link->permission.getOwner()->getPlayerNumber() - 1 == playerView) {
          if (link->getType() == 0) {
            color = Xwindow::Seagreen2;
          } else {
            color = Xwindow::Brown1;
          }
        } else {
          color = Xwindow::Gray50;
        }
      }

      window.fillRectangle(cellX, cellY, cellSize, cellSize, Xwindow::Black);
      window.fillRectangle(cellX + 1, cellY + 1, cellSize - 2, cellSize - 2,
                           color);

      if (cell.getOccupants().size() > 1) {
        auto link = std::dynamic_pointer_cast<Link>(cell.getOccupants()[1]);
        char linkName = link->getName();
        window.drawString(cellX + cellSize / 2 - 4, cellY + cellSize / 2 + 4,
                          std::string(1, linkName));
      }
    }
  }

  // Player 2 info
  y = boardY + (gridSize + 1) * cellSize;
  window.drawString(x, y, "Player 2:");
  y += 25;
  window.drawString(x, y, "Downloaded: 0D 0V"); // Hardcoded
  y += 25;
  window.drawString(x, y, "Abilities: 5"); // Hardcoded
  y += 25;
  auto p2Links = player2->getLinks();
  for (int i = 0; i < 4; ++i) {
    auto link = p2Links[i];
    std::string details;
    if (playerView == 1) { // P2's view
      details = std::string(1, link->getName()) + ": " + (link->getDetails());
    } else { // P1's view, hide P2's details
      details = std::string(1, link->getName()) + ": ??";
    }
    window.drawString(x + i * 100, y, details);
  }
  y += 25;
  for (int i = 4; i < 8; ++i) {
    auto link = p2Links[i];
    std::string details;
    if (playerView == 1) {
      details = std::string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      details = std::string(1, link->getName()) + ": ??";
    }
    window.drawString(x + (i - 4) * 100, y, details);
  }
}

GraphicsDisplay::~GraphicsDisplay() {
  // Graphics display cleanup
}

void GraphicsDisplay::printGame() {
  // Graphics game printing implementation
  Payload diff = getDiff();
  std::cout << "Diff: " << std::endl;
  std::cout << "Number of changes: " << diff.get("n") << std::endl;
  std::cout << "X coordinates: " << diff.get("x") << std::endl;
  std::cout << "Y coordinates: " << diff.get("y") << std::endl;
  std::cout << "New values: " << diff.get("news") << std::endl;

  for (int i = 0; i < diff.get("n"); i++) {
    int x = std::stoi(diff.get("x").at(i));
    int y = std::stoi(diff.get("y").at(i));
    std::string item = diff.get("news").at(i);

    if (item == "D") {
      drawCell(x, y, "D");
    } else if (item == "V") {
      drawCell(x, y, "V");
    }
    drawCell(x, y, item);
  }
}

void GraphicsDisplay::drawCell(int x, int y, string item) {}