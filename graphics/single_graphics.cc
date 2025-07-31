#include "single_graphics.h"
#include "../game/cell.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/payload.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Constructor
SingleGraphics::SingleGraphics(GameState &gameState, int playerView)
    : View(gameState, playerView), window{420, 800},
      gridSize{gameState.getBoard().getGridSize()}, cellSize{50} {
  // Fill the window with slategray1
  window.fillRectangle(0, 0, 420, 800, Xwindow::Slategray1);

  // Get the players
  auto players = gameState.getPlayers();
  auto player1 = players[0];
  auto player2 = players[1];

  // Player 1 info
  // Set the x and y positions
  int x = 15;
  int y = 50;
  // Draw the player 1 title
  window.drawString(x, y, "Player 1: *");
  // Increment the y position
  y += 25;
  // Draw the player 1 downloaded count
  window.drawString(x, y, "Downloaded: 0D 0V");
  // Increment the y position
  y += 25;
  // Draw the player 1 abilities count
  window.drawString(x, y, "Abilities: 5");
  // Increment the y position
  y += 25;

  // Get the player 1 links
  auto p1Links = player1->getLinks();
  // Draw the player 1 links
  for (int i = 0; i < 4; ++i) {
    // Get the link
    auto link = p1Links[i];
    // Get the details of the link
    std::string details;
    if (playerView == 0) { // P1's view
      details = std::string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      details = std::string(1, link->getName()) + ": ??";
    }
    // Draw the link details
    window.drawString(x + i * 100, y, details);
  }
  // Increment the y position
  y += 25;
  // Draw the player 1 links
  for (int i = 4; i < 8; ++i) {
    // Get the link
    auto link = p1Links[i];
    // Get the details of the link
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
      // Set the cell x and y positions
      int cellX = (j - 1) * cellSize + 10;
      int cellY = boardY + (i - 1) * cellSize;
      int color = Xwindow::White;
      window.fillRectangle(cellX, cellY, cellSize, cellSize, Xwindow::Black);

      Cell &cell = gameState.getBoard().getCell(Position{i, j});
      // If the cell is a server port
      if (cell.getType() == 1 || cell.getType() == 2) {
        // Set the color to gray30
        color = Xwindow::Gray30;
        // Fill the cell with gray30
        window.fillRectangle(cellX + 1, cellY + 1, cellSize - 2, cellSize - 2,
                             color);
        // Draw the server port
        window.drawString(cellX + cellSize / 2 - 4, cellY + cellSize / 2 + 4,
                          std::string(1, 'S'));
        // Continue to the next cell
        continue;
        // If the cell is a wall
      } else if (cell.getType() == -1) {
        window.fillRectangle(cellX + 1, cellY + 1, cellSize - 2, cellSize - 2,
                             Xwindow::Slategray1);
        continue;
      }

      // If the cell has more than one occupant
      if (cell.getOccupants().size() > 1) {
        // Get the link
        auto link = std::dynamic_pointer_cast<Link>(cell.getOccupants()[1]);
        size_t numPlayers = gameState.getPlayers().size();
        if (link->permission.getVisibleTo().size() >= numPlayers) {
          if (link->getType() == 0) { // Data link
            color = Xwindow::Seagreen2;
          } else { // Virus link
            color = Xwindow::Brown1;
          }
        } else {
          color = Xwindow::Gray50;
        }
      }

      // Fill the cell with the color
      window.fillRectangle(cellX + 1, cellY + 1, cellSize - 2, cellSize - 2,
                           color);

      // If the cell has more than one occupant
      if (cell.getOccupants().size() > 1) {
        // Get the link
        auto link = std::dynamic_pointer_cast<Link>(cell.getOccupants()[1]);
        // Draw the link name
        char linkName = link->getName();
        window.drawString(cellX + cellSize / 2 - 4, cellY + cellSize / 2 + 4,
                          std::string(1, linkName));
      }
    }
  }

  // Player 2 info
  // Set the y position
  y = boardY + (gridSize + 1) * cellSize;
  // Draw the player 2 title
  window.drawString(x, y, "Player 2:");
  // Increment the y position
  y += 25;
  // Draw the player 2 downloaded count
  window.drawString(x, y, "Downloaded: 0D 0V");
  // Increment the y position
  y += 25;
  // Draw the player 2 abilities count
  window.drawString(x, y, "Abilities: 5");
  // Increment the y position
  y += 25;
  // Get the player 2 links
  auto p2Links = player2->getLinks();
  // Draw the player 2 links
  for (int i = 0; i < 4; ++i) {
    // Get the link
    auto link = p2Links[i];
    // Get the details of the link
    string details;
    // If the player view is 1
    if (playerView == 1) {
      // P2's view
      details = string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      // P1's view, hide P2's details
      details = string(1, link->getName()) + ": ??";
    }
    // Draw the link details
    window.drawString(x + i * 100, y, details);
  }
  // Increment the y position
  y += 25;
  // Draw the player 2 links
  for (int i = 4; i < 8; ++i) {
    auto link = p2Links[i];
    // Get the details of the link
    string details;
    // If the player view is 1
    if (playerView == 1) {
      // P2's view
      details = string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      // P1's view, hide P2's details
      details = string(1, link->getName()) + ": ??";
    }
    // Draw the link details
    window.drawString(x + (i - 4) * 100, y, details);
  }
}

void SingleGraphics::updatePlayerInfo() {
  // Clear existing player info areas
  window.fillRectangle(0, 0, 420, 150, Xwindow::Slategray1);
  window.fillRectangle(0, 600, 420, 200, Xwindow::Slategray1);

  // Get the players
  auto players = gameState.getPlayers();
  // Get the player 1
  auto player1 = players[0];
  // Get the player 2
  auto player2 = players[1];
  // Get the current player number
  int curPlayerNum = gameState.getCurPlayer().getPlayerNumber();

  // Player 1 info
  // Set the x and y positions
  int x = 15;
  int y = 50;
  // Draw the player 1 title
  std::string p1Title = "Player 1:";
  // If the current player is player 1
  if (curPlayerNum == 1) {
    p1Title += " *";
  }
  // Draw the player 1 title
  window.drawString(x, y, p1Title);
  // Increment the y position
  y += 25;
  // Draw the player 1 downloaded count
  string p1Downloads =
      "Downloaded: " + std::to_string(player1->getScore().first) + "D " +
      std::to_string(player1->getScore().second) + "V";
  // Draw the player 1 downloaded count
  window.drawString(x, y, p1Downloads);
  // Increment the y position
  y += 25;
  // Draw the player 1 abilities count
  string p1Abilities =
      "Abilities: " + std::to_string(player1->getAbilities().size());
  // Draw the player 1 abilities count
  window.drawString(x, y, p1Abilities);
  // Increment the y position
  y += 25;
  // Get the player 1 links
  auto p1Links = player1->getLinks();
  // Draw the player 1 links
  for (int i = 0; i < 4; ++i) {
    // Get the link
    auto link = p1Links[i];
    // Get the details of the link
    string details;
    if (curPlayerNum == 1) {
      details = string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      if (link->permission.viewableBy(*player2)) {
        details = string(1, link->getName()) + ": " + (link->getDetails());
      } else {
        details = string(1, link->getName()) + ": ??";
      }
    }
    // Draw the link details
    window.drawString(x + i * 100, y, details);
  }
  // Increment the y position
  y += 25;
  // Draw the player 1 links
  for (int i = 4; i < 8; ++i) {
    // Get the link
    auto link = p1Links[i];
    // Get the details of the link
    string details;
    if (curPlayerNum == 1) {
      details = string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      if (link->permission.viewableBy(*player2)) {
        details = string(1, link->getName()) + ": " + (link->getDetails());
      } else {
        details = string(1, link->getName()) + ": ??";
      }
    }
    // Draw the link details
    window.drawString(x + (i - 4) * 100, y, details);
  }

  // Player 2 info
  // Set the y position
  y = 625;
  // Draw the player 2 title
  string p2Title = "Player 2:";
  // If the current player is player 2
  if (curPlayerNum == 2) {
    p2Title += " *";
  }
  // Draw the player 2 title
  window.drawString(x, y, p2Title);
  // Increment the y position
  y += 25;
  string p2Downloads =
      "Downloaded: " + std::to_string(player2->getScore().first) + "D " +
      std::to_string(player2->getScore().second) + "V";
  // Draw the player 2 downloaded count
  window.drawString(x, y, p2Downloads);
  // Increment the y position
  y += 25;
  string p2Abilities =
      "Abilities: " + std::to_string(player2->getAbilities().size());
  // Draw the player 2 abilities count
  window.drawString(x, y, p2Abilities);
  // Increment the y position
  y += 25;
  // Get the player 2 links
  auto p2Links = player2->getLinks();
  // Draw the player 2 links
  for (int i = 0; i < 4; ++i) {
    auto link = p2Links[i];
    // Get the details of the link
    string details;
    if (curPlayerNum == 2) {
      details = string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      if (link->permission.viewableBy(*player1)) {
        details = string(1, link->getName()) + ": " + (link->getDetails());
      } else {
        details = string(1, link->getName()) + ": ??";
      }
    }
    // Draw the link details
    window.drawString(x + i * 100, y, details);
  }
  // Increment the y position
  y += 25;
  for (int i = 4; i < 8; ++i) {
    auto link = p2Links[i];
    // Get the details of the link
    string details;
    if (curPlayerNum == 2) {
      details = string(1, link->getName()) + ": " + (link->getDetails());
    } else {
      if (link->permission.viewableBy(*player1)) {
        details = string(1, link->getName()) + ": " + (link->getDetails());
      } else {
        details = string(1, link->getName()) + ": ??";
      }
    }
    // Draw the link details
    window.drawString(x + (i - 4) * 100, y, details);
  }
}

// Print the game
void SingleGraphics::printGame() {
  updatePlayerInfo();
  // Get the diff
  Payload diff = getDiff();
  // Get the x
  string x = diff.get("x");
  // Get the y
  string y = diff.get("y");
  // Get the news
  string news = diff.get("news");
  std::istringstream xStream(x);
  std::istringstream yStream(y);
  std::istringstream newsStream(news);

  // Draw the cells
  for (int i = 0; i < stoi(diff.get("n")); i++) {
    // Get the x and y values
    int xVal, yVal;
    char item;
    // Get the x value
    xStream >> xVal;
    // Get the y value
    yStream >> yVal;
    // Get the news
    newsStream >> item;
    // Draw the cell
    drawCell(xVal, yVal, item);
  }
}

// Draw the cell
// Draw the cell
void SingleGraphics::drawCell(int x, int y, char item) {
  // Get the x and y positions
  int xPos = (y - 1) * cellSize + 10;
  // Get the y position
  int yPos = (x - 1) * cellSize + 175;

  // Fill the cell with black
  window.fillRectangle(xPos, yPos, cellSize, cellSize, Xwindow::Black);

  // If the item is a dot
  if (item == '.') {
    window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                         Xwindow::White);
    return;
  }
  // If the item is a link
  else if (item >= 'a' && item <= 'h' || item >= 'A' && item <= 'H') {
    // Get the link
    auto link = gameState.getLink(item);
    size_t numPlayers = gameState.getPlayers().size();

    if (link->permission.getVisibleTo().size() >= numPlayers) {
      if (link->getType() == 0) { // Data link
        window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                             Xwindow::Seagreen2);
      } else { // Virus link
        window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                             Xwindow::Brown1);
      }
    } else {
      window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                           Xwindow::Gray50);
    }
  }
  // If the item is a server port
  else if (item == 'S') {
    // Fill the cell with gray30
    window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                         Xwindow::Gray30);
  }
  // If the item is a wall
  else if (item == 'w' || item == 'm') {
    // Fill the cell with darkorange
    window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                         Xwindow::Darkorange);
  }
  // If the item is a wall
  else if (item == 'x') {
    // Fill the cell with gray75
    window.fillRectangle(xPos + 1, yPos + 1, cellSize - 2, cellSize - 2,
                         Xwindow::Gray75);
  }
  // Draw the item
  window.drawString(xPos + cellSize / 2 - 4, yPos + cellSize / 2 + 4,
                    std::string(1, item));
}
