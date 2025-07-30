#include "graphics_display.h"
#include "../game/cell.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/payload.h"
#include <iostream>
#include <sstream>
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
        window.fillRectangle(cellX, cellY, cellSize, cellSize, Xwindow::Black);
        window.fillRectangle(cellX + 1, cellY + 1, cellSize - 2, cellSize - 2,
                             color);
        window.drawString(cellX + cellSize / 2 - 4, cellY + cellSize / 2 + 4,
                          std::string(1, 'S'));
        continue;
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
    string details;
    if (playerView == 1) { // P2's view
      details = string(1, link->getName()) + ": " + (link->getDetails());
    } else { // P1's view, hide P2's details
      details = string(1, link->getName()) + ": ??";
    }
    window.drawString(x + i * 100, y, details);
  }
  y += 25;
  for (int i = 4; i < 8; ++i) {
    auto link = p2Links[i];
    string details;
    if (playerView == 1) {
      details = string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      details = string(1, link->getName()) + ": ??";
    }
    window.drawString(x + (i - 4) * 100, y, details);
  }
}

GraphicsDisplay::~GraphicsDisplay() {
  // Graphics display cleanup
}

void GraphicsDisplay::updatePlayerInfo() {
  // Clear existing player info areas
  window.fillRectangle(0, 0, 420, 150, Xwindow::Seashell1);
  window.fillRectangle(0, 600, 420, 200, Xwindow::Seashell1);

  auto players = gameState.getPlayers();
  auto player1 = players[0];
  auto player2 = players[1];
  int curPlayerNum = gameState.getCurPlayer().getPlayerNumber();

  // Player 1 info
  int x = 15;
  int y = 50;
  std::string p1Title = "Player 1:";
  if (curPlayerNum == 1) {
    p1Title += " *";
  }
  window.drawString(x, y, p1Title);

  y += 25;
  string p1Downloads =
      "Downloaded: " + std::to_string(player1->getScore().first) + "D " +
      std::to_string(player1->getScore().second) + "V";
  window.drawString(x, y, p1Downloads);

  y += 25;
  string p1Abilities =
      "Abilities: " + std::to_string(player1->getAbilities().size());
  window.drawString(x, y, p1Abilities);

  y += 25;
  auto p1Links = player1->getLinks();
  for (int i = 0; i < 4; ++i) {
    auto link = p1Links[i];
    string details;
    if (playerView == 0) {
      details = string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      if (link->permission.viewableBy(*player2)) {
        details = string(1, link->getName()) + ": " + (link->getDetails());
      } else {
        details = string(1, link->getName()) + ": ??";
      }
    }
    window.drawString(x + i * 100, y, details);
  }
  y += 25;
  for (int i = 4; i < 8; ++i) {
    auto link = p1Links[i];
    string details;
    if (playerView == 0) {
      details = string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      if (link->permission.viewableBy(*player2)) {
        details = string(1, link->getName()) + ": " + (link->getDetails());
      } else {
        details = string(1, link->getName()) + ": ??";
      }
    }
    window.drawString(x + (i - 4) * 100, y, details);
  }

  // Player 2 info
  y = 625;
  string p2Title = "Player 2:";
  if (curPlayerNum == 2) {
    p2Title += " *";
  }
  window.drawString(x, y, p2Title);

  y += 25;
  string p2Downloads =
      "Downloaded: " + std::to_string(player2->getScore().first) + "D " +
      std::to_string(player2->getScore().second) + "V";
  window.drawString(x, y, p2Downloads);

  y += 25;
  string p2Abilities =
      "Abilities: " + std::to_string(player2->getAbilities().size());
  window.drawString(x, y, p2Abilities);

  y += 25;
  auto p2Links = player2->getLinks();
  for (int i = 0; i < 4; ++i) {
    auto link = p2Links[i];
    string details;
    if (playerView == 1) {
      details = string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      if (link->permission.viewableBy(*player1)) {
        details = string(1, link->getName()) + ": " + (link->getDetails());
      } else {
        details = string(1, link->getName()) + ": ??";
      }
    }
    window.drawString(x + i * 100, y, details);
  }
  y += 25;
  for (int i = 4; i < 8; ++i) {
    auto link = p2Links[i];
    string details;
    if (playerView == 1) {
      details = string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      if (link->permission.viewableBy(*player1)) {
        details = string(1, link->getName()) + ": " + (link->getDetails());
      } else {
        details = string(1, link->getName()) + ": ??";
      }
    }
    window.drawString(x + (i - 4) * 100, y, details);
  }
}
void GraphicsDisplay::printGame() {
  updatePlayerInfo();
  Payload diff = getDiff();
  string x = diff.get("x");
  string y = diff.get("y");
  string news = diff.get("news");
  std::istringstream xStream(x);
  std::istringstream yStream(y);
  std::istringstream newsStream(news);

  for (int i = 0; i < stoi(diff.get("n")); i++) {
    int xVal, yVal;
    char item;
    xStream >> xVal;
    yStream >> yVal;
    newsStream >> item;
    drawCell(xVal, yVal, item);
  }
}

void GraphicsDisplay::drawCell(int x, int y, char item) {

  int xPos = (y - 1) * cellSize + 10;
  int yPos = (x - 1) * cellSize + 175;

  window.fillRectangle(xPos, yPos, cellSize, cellSize, Xwindow::Black);

  if (item == '.') {
    window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                         Xwindow::White);
    return;
  } else if (item >= 'a' && item <= 'h' || item >= 'A' && item <= 'H') {
    auto link = gameState.getLink(item);
    auto players = gameState.getPlayers();
    auto curPlayer = players[playerView];

    if (link->permission.getOwner()->getPlayerNumber() - 1 == playerView) {
      if (link->getType() == 0) {
        window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                             Xwindow::Seagreen2);
      } else {
        window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                             Xwindow::Brown1);
      }
    } else {
      if (link->permission.viewableBy(*curPlayer)) {
        if (link->getType() == 0) {
          window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                               Xwindow::Darkseagreen1);
        } else {
          window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                               Xwindow::Lightcoral);
        }
      } else {
        window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                             Xwindow::Gray50);
      }
    }

  } else if (item == 'S') {
    window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                         Xwindow::Gray30);
  }
  window.drawString(xPos + cellSize / 2 - 4, yPos + cellSize / 2 + 4,
                    std::string(1, item));
}