#include "view.h"
#include "../ability/ability.h"
#include "../game/board.h"
#include "../game/cell.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/game_event.h"
#include "../utils/payload.h"
#include <iostream>

View::View(GameState &gameState, int playerView)
    : playerView(playerView), gameState(gameState),
      gridSize(gameState.getBoard().getGridSize()),
      oldBoard(std::vector<std::vector<std::string>>(
          gridSize, std::vector<std::string>(gridSize, "."))),
      board(std::vector<std::vector<std::string>>(
          gridSize, std::vector<std::string>(gridSize, "."))),
      linksOnBoard(std::vector<std::vector<std::string>>(
          gridSize, std::vector<std::string>(gridSize, ""))),
      usedAbilities(0),
      unusedAbilities(gameState.getCurPlayer().getAbilities().size()),
      downloadedData(std::vector<int>(gameState.getPlayers().size(), 0)),
      downloadedVirus(std::vector<int>(gameState.getPlayers().size(), 0)) {
  // Initialize the board
  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {
      // Get the type of the cell
      int type = gameState.getBoard().getCell(Position(i + 1, j + 1)).getType();
      if (type >= 1 && type <= 10) {
        // If the type is a server port
        board[i][j] = "S";
      } else if (type >= 11 && type <= 20) {
        // If the type is a wall
        board[i][j] = "=";
      } else if (type == -1) {
        // If the type is a space
        board[i][j] = " ";
      }
    }
  }

  // Initialize the links on board
  for (int i = 0; i < gameState.getPlayers().size(); i++) {
    for (int j = 0; j < gameState.getPlayers()[i]->getLinks().size(); j++) {
      Link &link = *gameState.getPlayers()[i]->getLinks()[j];
      // Get the position of the link
      linksOnBoard[link.getPosition().getPosition().first - 1]
                  [link.getPosition().getPosition().second - 1] =
                      link.getName();
    }
  }

  // Initialize the old board
  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {
      oldBoard[i][j] =
          (linksOnBoard[i][j] != "" ? linksOnBoard[i][j] : board[i][j]);
    }
  }
}

// Store the old board
void View::storeOldboard() {
  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {
      // Store the old board
      oldBoard[i][j] =
          (linksOnBoard[i][j] != "" ? linksOnBoard[i][j] : board[i][j]);
    }
  }
}

// Notify the view of the event
void View::notify(GameEvent &event) {
  // Store the old board
  storeOldboard();
  // If the event is a link moved
  if (event.getEventType() == EventType::LinkMoved) {
    int oldX = std::stoi(event.getPayload().get("oldX")) - 1;
    int oldY = std::stoi(event.getPayload().get("oldY")) - 1;
    int newX = std::stoi(event.getPayload().get("newX")) - 1;
    int newY = std::stoi(event.getPayload().get("newY")) - 1;
    // Get the old link
    string oldLink = linksOnBoard[oldX][oldY];
    // Set the old link to an empty string
    linksOnBoard[oldX][oldY] = "";
    // If the new x and y are within the grid
    if (newX >= 0 && newX < gridSize && newY >= 0 && newY < gridSize) {
      linksOnBoard[newX][newY] = oldLink;
    }
  }
  // If the event is a link downloaded
  else if (event.getEventType() == EventType::LinkDownloaded) {
    // Get the x and y of the link
    int x = std::stoi(event.getPayload().get("x")) - 1;
    int y = std::stoi(event.getPayload().get("y")) - 1;
    int player = std::stoi(event.getPayload().get("player")) - 1;
    string type = event.getPayload().get("type");
    // If the type is a data link
    if (type == "0") {
      // Increment the downloaded data
      downloadedData[player]++;
    }
    // If the type is a virus link
    else if (type == "1") {
      // Increment the downloaded virus
      downloadedVirus[player]++;
    }
    if (x >= 0 && x < gridSize && y >= 0 && y < gridSize) {
      linksOnBoard[x][y] = "";
    }
  }
  // If the event is a ability used
  else if (event.getEventType() == EventType::AbilityUsed) {
    // Get the player
    int player = std::stoi(event.getPayload().get("player"));
    // Increment the used abilities
    usedAbilities++;
    // Decrement the unused abilities
    unusedAbilities--;
  }
  // If the event is a ability placed
  else if (event.getEventType() == EventType::AbilityPlaced) {
    // Get the x and y of the ability
    int x = std::stoi(event.getPayload().get("x")) - 1;
    int y = std::stoi(event.getPayload().get("y")) - 1;
    string marker = event.getPayload().get("marker");
    // Increment the used abilities
    usedAbilities++;
    // Decrement the unused abilities
    unusedAbilities--;
    // Set the ability to the marker
    board[x][y] = marker[0];
  }
  // If the event is a occupant added
  else if (event.getEventType() == EventType::OccupantAdded) {
    // Get the x and y of the occupant
    int x = std::stoi(event.getPayload().get("x")) - 1;
    int y = std::stoi(event.getPayload().get("y")) - 1;
    string occupant = event.getPayload().get("occupant");
    // Set the occupant to the link on board
    linksOnBoard[x][y] = occupant[0];
  }
  // If the event is a occupant removed
  else if (event.getEventType() == EventType::OccupantRemoved) {
    // Get the x and y of the occupant
    int x = std::stoi(event.getPayload().get("x")) - 1;
    int y = std::stoi(event.getPayload().get("y")) - 1;
    // Set the link on board to an empty string
    linksOnBoard[x][y] = "";
  }
  // If the event is a battle triggered
  else if (event.getEventType() == EventType::BattleTriggered) {
    int x = std::stoi(event.getPayload().get("x")) - 1;
    int y = std::stoi(event.getPayload().get("y")) - 1;
    string winner = event.getPayload().get("winner");
    linksOnBoard[x][y] = winner[0];
  }
  // If the event is a polarize
  else if (event.getEventType() == EventType::Polarize) {
    // Get the x and y of the polarize
    int x = std::stoi(event.getPayload().get("x")) - 1;
    int y = std::stoi(event.getPayload().get("y")) - 1;
    // Set the old board to a dot
    oldBoard[x][y] = ".";
  }
}

// Get the diff
Payload View::getDiff() {
  // Create a payload
  Payload diff;
  // Create a string for the news
  std::string news;
  // Create a string for the x
  std::string xs;
  std::string ys;

  // Loop through the grid
  for (int i = 0; i < gridSize; i++) {
    // Loop through the grid
    for (int j = 0; j < gridSize; j++) {
      // If the old board is not the same as the links on board
      if (oldBoard[i][j] !=
          (linksOnBoard[i][j] != "" ? linksOnBoard[i][j] : board[i][j])) {
        // Add the x to the string
        xs += std::to_string(i + 1) + " ";
        // Add the y to the string
        ys += std::to_string(j + 1) + " ";
        // Add the news to the string
        news +=
            (linksOnBoard[i][j] != "" ? linksOnBoard[i][j] : board[i][j]) + " ";
      }
    }
  }

  // Add the number of news to the payload
  diff.add("n", std::to_string(news.size() / 2));
  // Add the x to the payload
  diff.add("x", xs);
  // Add the y to the payload
  diff.add("y", ys);
  // Add the news to the payload
  diff.add("news", news);

  return diff;
}

// Print the abilities
void View::printAbilities(Player &playerDisplay) {
  // Loop through the players
  for (int i = 0; i < gameState.getPlayers().size(); i++) {
    // Get the current player
    Player &curPlayer = *gameState.getPlayers()[i];
    // Print the player number
    std::cout << "Player " << i + 1 << ":" << std::endl;
    // Print the used abilities
    std::cout << "Used: ";
    // Loop through the abilities
    for (int j = 0; j < curPlayer.getAbilities().size(); j++) {
      // Get the ability
      Ability &ability = *curPlayer.getAbilities()[j];
      // If the ability is used
      if (ability.used) {
        // Print the ability
        std::cout << (ability.permission.viewableBy(playerDisplay)
                          ? ability.name
                          : "? ")
                  << " ";
      }
    }
    std::cout << std::endl;
    // Print the unused abilities
    std::cout << "Unused: ";
    // Loop through the abilities
    for (int j = 0; j < curPlayer.getAbilities().size(); j++) {
      // Get the ability
      Ability &ability = *curPlayer.getAbilities()[j];
      // If the ability is not used
      if (!ability.used) {
        // Print the ability
        std::cout << (ability.permission.viewableBy(playerDisplay)
                          ? ability.name
                          : "? ")
                  << " ";
      }
    }
    // Print a new line
    std::cout << std::endl << std::endl;
  }
}

