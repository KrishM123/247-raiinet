#include "game_state.h"
#include "../controller/event_types.h"
#include "../utils/message_queue.h"
#include "../utils/payload.h"
#include "../utils/permission.h"
#include "board.h"
#include "cell.h"
#include "gameTriggers/battleTrigger.h"
#include "gameTriggers/edgeTrigger.h"
#include "gameTriggers/serverTrigger.h"
#include "link.h"
#include "occupant.h"
#include "player.h"
#include <iostream>
#include <stdexcept>

using namespace std;

GameState::GameState(int numPlayers, int boardSize, vector<string> links,
                     vector<string> abilities)
    : board{Board{boardSize}}, isGameOver{false} {
  for (int i = 0; i < numPlayers; i++) {
    players.push_back(make_shared<Player>(i + 1, links[i], abilities[i]));
    players[i]->initLinks(links[i], Permission(players[i]));
    players[i]->initAbilities(abilities[i], Permission(players[i]), *this);
  }
  curPlayer = players[0];
}

void GameState::init() {
  // initialize board cells
  std::cout << "initializing board" << std::endl;
  for (int i = 0; i < board.getGridSize() + 2; i++) {
    for (int j = 0; j < board.getGridSize() + 2; j++) {
      if (j == 0 || j == board.getGridSize() + 1) {
        board.getCell(Position{i, j}).setType(-1);
        continue;
      }
      if (i == 0) {
        board.getCell(Position{i, j}).setType(11);
        board.placeOccupant(make_shared<EdgeTrigger>(*this, Position{i, j},
                                                     Permission(players[0])),
                            Position{i, j});
      } else if (i == board.getGridSize() + 1) {
        board.getCell(Position{i, j}).setType(12);
        board.placeOccupant(make_shared<EdgeTrigger>(*this, Position{i, j},
                                                     Permission(players[1])),
                            Position{i, j});
      } else {
        int type = 0;
        if (i == 1 && (j == board.getGridSize() / 2 ||
                       j == board.getGridSize() / 2 - 1)) {
          type = 1;
          board.placeOccupant(
              make_shared<ServerTrigger>(*this, Position{i, j},
                                         Permission(players[0])),
              Position{i, j});
        } else if (i == board.getGridSize() &&
                   (j == board.getGridSize() / 2 ||
                    j == board.getGridSize() / 2 - 1)) {
          type = 2;
          board.placeOccupant(
              make_shared<ServerTrigger>(*this, Position{i, j},
                                         Permission(players[1])),
              Position{i, j});
        } else {
          board.placeOccupant(make_shared<BattleTrigger>(*this, Position{i, j}),
                              Position{i, j});
        }
        board.getCell(Position{i, j}).setType(type);
      }
    }
  }
  std::cout << "board initialized" << std::endl;
  // place links on board
  for (int i = 0; i < players.size(); i++) {
    auto playerLinks = players[i]->getLinks();
    for (int j = 0; j < playerLinks.size(); j++) {
      if (i == 0) {
        if (j == 3 || j == 4) {
          board.placeOccupant(playerLinks[j], Position{2, j});
        } else {
          board.placeOccupant(playerLinks[j], Position{1, j});
        }
      } else if (i == 1) {
        if (j == 3 || j == 4) {
          board.placeOccupant(playerLinks[j],
                              Position{board.getGridSize() - 1, j});
        } else {
          board.placeOccupant(playerLinks[j], Position{board.getGridSize(), j});
        }
      }
    }
  }
  std::cout << "links placed" << std::endl;
}

GameState::~GameState() {};

vector<shared_ptr<Link>> GameState::getLinks() {
  vector<shared_ptr<Link>> links;
  for (auto player : players) {
    vector<shared_ptr<Link>> playerLinks = player->getLinks();
    links.insert(links.end(), playerLinks.begin(), playerLinks.end());
  }
  return links;
}

shared_ptr<Link> GameState::getLink(char name) {
  for (auto &player : players) {
    auto it = player->linksMap.find(name);
    if (it != player->linksMap.end()) {
      return player->getLinks()[it->second];
    }
  }
  return nullptr;
}

Board &GameState::getBoard() { return board; }

vector<shared_ptr<Player>> GameState::getPlayers() { return players; }

Player &GameState::getCurPlayer() const { return *curPlayer; }

bool GameState::isWon() const { return getWinner() != nullptr; }

shared_ptr<Player> GameState::getWinner() const {
  for (const auto &player : players) {
    if (player->getScore().first >= 4) {
      return player;
    }
  }

  int numLost = 0;
  shared_ptr<Player> lastStanding = nullptr;
  for (const auto &player : players) {
    if (player->getScore().second >= 4) {
      numLost++;
    } else {
      lastStanding = player;
    }
  }

  if (numLost == players.size() - 1) {
    return lastStanding;
  }

  return nullptr;
}

void GameState::moveLink(shared_ptr<Link> link, string direction) {
  Position oldPos = link->getPosition();
  map<string, Position> possibleMoves = link->getMoves();

  if (possibleMoves.find(direction) == possibleMoves.end() ||
      !board.isValidPosition(oldPos + possibleMoves[direction],
                             curPlayer->getPlayerNumber())) {
    throw invalid_argument("Invalid move: " + direction);
  }

  Position newPos = oldPos + possibleMoves[direction];

  board.removeOccupant(link, oldPos);
  board.placeOccupant(link, newPos);

  notifyLinkMoved(link, oldPos, newPos);
}

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

void GameState::addOccupant(std::shared_ptr<Occupant> occupant,
                            const Position &pos) {
  board.placeOccupant(occupant, pos);
}

void GameState::removeOccupant(std::shared_ptr<Occupant> occupant,
                               const Position &pos) {
  board.removeOccupant(occupant, pos);
}

void GameState::endGame() {
  isGameOver = true;
  notifyGameOver();
}

void GameState::downloadLink(shared_ptr<Link> link,
                             shared_ptr<Player> downloader) {
  link->permission.addViewer(downloader);
  vector<shared_ptr<Link>> downloadedLinks = downloader->getDownloadedLinks();
  downloadedLinks.push_back(link);
  downloader->setDownloadedLinks(downloadedLinks);

  link->setIsDownloaded(true);
  removeOccupant(link, link->getPosition());
}

void GameState::notifyNextTurn() {
  // TODO: Implement turn notification
}

void GameState::nextTurn() {
  // Find the next player
  int currentIndex = -1;
  for (int i = 0; i < players.size(); i++) {
    if (players[i] == curPlayer) {
      currentIndex = i;
      break;
    }
  }

  if (currentIndex != -1) {
    int nextIndex = (currentIndex + 1) % players.size();
    curPlayer = players[nextIndex];
    notifyNextTurn();
  }
}

void GameState::notifyGameOver() {
  // TODO: Implement game over notification
}