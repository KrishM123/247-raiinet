#include "game_state.h"
#include "../utils/event_types.h"
#include "../utils/message_queue.h"
#include "../utils/payload.h"
#include "../utils/permission.h"
#include "board.h"
#include "cell.h"
#include "gameTriggers/battleTrigger.h"
#include "gameTriggers/edgeTrigger.h"
#include "gameTriggers/serverTrigger.h"
#include "gameTriggers/sideTrigger.h"
#include "link.h"
#include "occupant.h"
#include "player.h"
#include <stdexcept>

using namespace std;

GameState::GameState(int numPlayers, int boardSize, vector<string> links,
                     vector<string> abilities, bool obstaclesEnabled,
                     bool sideMovesEnabled)
    : board{Board{boardSize}}, isGameOver{false},
      obstaclesEnabled{obstaclesEnabled}, sideMovesEnabled{sideMovesEnabled} {
  // Initialize players
  for (int i = 0; i < numPlayers; i++) {
    players.push_back(make_shared<Player>(i + 1, links[i], abilities[i]));
    players[i]->initLinks(links[i], Permission(players[i]));
    players[i]->initAbilities(abilities[i], Permission(players[i]), *this);
  }
  // Set current player to first player
  curPlayer = players[0];
}

void GameState::init() {
  // Initialize board cells
  for (int i = 0; i < board.getGridSize() + 2; i++) {
    for (int j = 0; j < board.getGridSize() + 2; j++) {
      if (j == 0 || j == board.getGridSize() + 1) {
        // If side moves are enabled, set the cell to empty
        if (sideMovesEnabled) {
          board.getCell(Position{i, j}).setType(0);
          board.placeOccupant(make_shared<SideTrigger>(*this, Position{i, j}),
                              Position{i, j});
        } else {
          // If side moves are not enabled, set the cell to -1
          board.getCell(Position{i, j}).setType(-1);
        }
      } else if (i == 0) {
        // If the cell is on the top edge, set the cell to 11
        board.getCell(Position{i, j}).setType(11);
        board.placeOccupant(make_shared<EdgeTrigger>(*this, Position{i, j},
                                                     Permission(players[0])),
                            Position{i, j});
      } else if (i == board.getGridSize() + 1) {
        // If the cell is on the bottom edge, set the cell to 12
        board.getCell(Position{i, j}).setType(12);
        board.placeOccupant(make_shared<EdgeTrigger>(*this, Position{i, j},
                                                     Permission(players[1])),
                            Position{i, j});
      } else {
        // If the cell is not on the edge, set the cell to 0
        int type = 0;
        if (i == 1 && (j == board.getGridSize() / 2 ||
                       j == board.getGridSize() / 2 + 1)) {
          // If the cell is on the server cell, set the cell to 1
          type = 1;
          board.placeOccupant(
              make_shared<ServerTrigger>(*this, Position{i, j},
                                         Permission(players[0])),
              Position{i, j});
        } else if (i == board.getGridSize() &&
                   (j == board.getGridSize() / 2 ||
                    j == board.getGridSize() / 2 + 1)) {
          // If the cell is on the server cell, set the cell to 2
          type = 2;
          board.placeOccupant(
              make_shared<ServerTrigger>(*this, Position{i, j},
                                         Permission(players[1])),
              Position{i, j});
        } else {
          // If the cell is not on the server cell, set the cell to 0
          board.placeOccupant(make_shared<BattleTrigger>(*this, Position{i, j}),
                              Position{i, j});
        }
        board.getCell(Position{i, j}).setType(type);
      }
    }
  }
  // If obstacles are enabled, set 3 random spaces to be empty (-1), avoiding first and last rows
  if (obstaclesEnabled) {
    srand(time(nullptr));
    int count = 0;
    while (count < 3) {
      int i = (rand() % (board.getGridSize() - 5)) +
              3; // Range from row 3 to size-1
      int j = (rand() % board.getGridSize()) + 1;
      Cell &cell = board.getCell(Position{i, j});
      // Skip if cell is a server
      if (cell.getType() == -1) {
        continue;
      }
      cell.setType(-1);
      count++;
    }
  }
  // Place links on board
  for (int i = 0; i < players.size(); i++) {
    auto playerLinks = players[i]->getLinks();
    for (int j = 0; j < playerLinks.size(); j++) {
      // If the player is player 1
      if (i == 0) {
        // If the link is on the server cell, set the cell to 1
        if (j == 3 || j == 4) {
          board.placeOccupant(playerLinks[j], Position{2, j + 1});
        } else {
          board.placeOccupant(playerLinks[j], Position{1, j + 1});
        }
      } else if (i == 1) {
        // If the link is on the server cell, set the cell to 2
        if (j == 3 || j == 4) {
          board.placeOccupant(playerLinks[j],
                              Position{board.getGridSize() - 1, j + 1});
        } else {
          board.placeOccupant(playerLinks[j],
                              Position{board.getGridSize(), j + 1});
        }
      }
    }
  }
}

// Get the link with the given name
shared_ptr<Link> GameState::getLink(char name) {
  // Search for the link in all players' links
  for (auto &player : players) {
    auto it = player->linksMap.find(name);
    if (it != player->linksMap.end()) {
      // If the link is found, return it
      return player->getLinks()[it->second];
    }
  }
  // If the link is not found, throw an error
  throw invalid_argument("Link not found");
  return nullptr;
}

// Get the board
Board &GameState::getBoard() { return board; }

// Get the players
vector<shared_ptr<Player>> GameState::getPlayers() { return players; }

// Get the current player
Player &GameState::getCurPlayer() const { return *curPlayer; }

// Check if the game is won
bool GameState::isWon() const { return getWinner() != nullptr; }

// Get the winner
shared_ptr<Player> GameState::getWinner() const {
  // Check if any player has 4 wins
  for (const auto &player : players) {
    if (player->getScore().first >= 4) {
      return player;
    }
  }

  // Check if any player has 4 losses
  int numLost = 0;
  shared_ptr<Player> lastStanding = nullptr;
  for (const auto &player : players) {
    if (player->getScore().second >= 4) {
      numLost++;
    } else {
      lastStanding = player;
    }
  }

  // If all but one player has 4 losses, return the last standing player
  if (numLost == players.size() - 1) {
    return lastStanding;
  }

  // If no player has 4 losses, return nullptr
  return nullptr;
}

// Move a link in the given direction
void GameState::moveLink(shared_ptr<Link> link, string direction) {
  // Get the old position of the link
  Position oldPos = link->getPosition();
  // Get the possible moves of the link
  map<string, Position> possibleMoves = link->getMoves();

  // If the move is invalid, throw an error
  if (possibleMoves.find(direction) == possibleMoves.end() ||
      link->getIsDownloaded()) {
    throw invalid_argument("Invalid move: " + direction);
  }

  // Get the new position of the link
  Position newPos = oldPos + possibleMoves[direction];

  // Cap row and col to be in the board + 2 buffer (for link boost error)
  int newRow = newPos.getPosition().first;
  int curPlayerNum = getCurPlayer().getPlayerNumber();

  // Cap the row to be in the board + 2 buffer (for link boost error)
  if (curPlayerNum == 1) {
    if (newRow > board.getGridSize() + 1)
      newRow = board.getGridSize() + 1;
  } else if (curPlayerNum == 2) {
    if (newRow < 0)
      newRow = 0;
  }
  newPos.setPosition(newRow, newPos.getPosition().second);

  // If the new position is invalid, throw an error
  if (!board.isValidPosition(newPos, curPlayerNum)) {
    throw invalid_argument("Invalid move: " + direction);
  }

  // Notify the link moved
  notifyLinkMoved(link, oldPos, newPos);

  // Remove the link from the old position and add it to the new position
  board.removeOccupant(link, oldPos);
  board.placeOccupant(link, newPos);
}

// Notify the link moved
void GameState::notifyLinkMoved(shared_ptr<Link> link, Position oldPos,
                                Position newPos) {
  auto queue = MessageQueue::getInstance();
  map<string, string> payloadMap = {
      {"oldX", to_string(oldPos.getPosition().first)},
      {"oldY", to_string(oldPos.getPosition().second)},
      {"newX", to_string(newPos.getPosition().first)},
      {"newY", to_string(newPos.getPosition().second)}};
  Payload payload{payloadMap};
  EventType eventType = EventType::LinkMoved;
  queue->enqueueEvent(GameEvent(eventType, payload));
}

// Add an occupant to the board
void GameState::addOccupant(std::shared_ptr<Occupant> occupant,
                            const Position &pos) {
  board.placeOccupant(occupant, pos);
}

// Remove an occupant from the board
void GameState::removeOccupant(std::shared_ptr<Occupant> occupant,
                               const Position &pos) {
  board.removeOccupant(occupant, pos);
}

// End the game
void GameState::endGame() { isGameOver = true; }

// Download a link
void GameState::downloadLink(shared_ptr<Link> link,
                             shared_ptr<Player> downloader) {
  for (auto player : players) {
    if (player == downloader) {
      link->permission.addViewer(player);
    }
  }

  vector<shared_ptr<Link>> downloadedLinks = downloader->getDownloadedLinks();
  downloadedLinks.push_back(link);
  downloader->setDownloadedLinks(downloadedLinks);

  link->setIsDownloaded(true);
  removeOccupant(link, link->getPosition());
  notifyLinkDownloaded(link, downloader);
}

// Notify the link downloaded
void GameState::notifyLinkDownloaded(shared_ptr<Link> link,
                                     shared_ptr<Player> downloader) {
  auto queue = MessageQueue::getInstance();
  map<string, string> payloadMap;
  payloadMap["x"] = to_string(link->getPosition().getPosition().first);
  payloadMap["y"] = to_string(link->getPosition().getPosition().second);
  payloadMap["player"] = to_string(downloader->getPlayerNumber());
  payloadMap["type"] = to_string(link->getType());
  Payload payload{payloadMap};
  EventType eventType = EventType::LinkDownloaded;
  queue->enqueueEvent(GameEvent(eventType, payload));
}

// Next turn
void GameState::nextTurn() {
  // Find the next player
  int currentIndex = -1;
  for (int i = 0; i < players.size(); i++) {
    if (players[i] == curPlayer) {
      currentIndex = i;
      break;
    }
  }

  // If the current player is found, set the next player
  if (currentIndex != -1) {
    int nextIndex = (currentIndex + 1) % players.size();
    curPlayer = players[nextIndex];
  }
}
