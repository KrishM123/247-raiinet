#include "game_state.h"
#include "player.h"
#include "link.h"
#include "occupant.h"
#include <stdexcept>

using namespace std;

GameState::GameState(int numPlayers, int boardSize, vector<string> links, vector<string> abilities) : board{boardSize}
{
  for (int i = 0; i < numPlayers; i++)
  {
    players.push_back(make_shared<Player>(i + 1, links[i], abilities[i]));
  }
  curPlayer = players[0];
}

vector<shared_ptr<Link>> GameState::getLinks()
{
  vector<shared_ptr<Link>> links;
  for (auto player : players)
  {
    vector<shared_ptr<Link>> playerLinks = player->getLinks();
    links.insert(links.end(), playerLinks.begin(), playerLinks.end());
  }
  return links;
}

Board &GameState::getBoard()
{
  return board;
}

vector<shared_ptr<Player>> GameState::getPlayers()
{
  return players;
}

Player &GameState::getCurPlayer()
{
  return *curPlayer;
}

bool GameState::isWon() const
{
  return getWinner() != nullptr;
}

shared_ptr<Player> GameState::getWinner() const
{
  for (const auto &player : players)
  {
    if (player->getScore().first >= 4)
    {
      return player;
    }
  }

  int numLost = 0;
  shared_ptr<Player> lastStanding = nullptr;
  for (const auto &player : players)
  {
    if (player->getScore().second >= 4)
    {
      numLost++;
    }
    else
    {
      lastStanding = player;
    }
  }

  if (numLost == players.size() - 1)
  {
    return lastStanding;
  }

  return nullptr;
}

void GameState::moveLink(std::shared_ptr<Link> link, std::string direction)
{
  Position oldPos = link->getPosition();
  map<string, Position> possibleMoves = link->getMoves();

  if (possibleMoves.find(direction) == possibleMoves.end() || !board.isValidPosition(oldPos + possibleMoves[direction]))
  {
    throw invalid_argument("Invalid move: " + direction);
  }

  Position newPos = oldPos + possibleMoves[direction];

  board.removeOccupant(link, oldPos);
  board.placeOccupant(link, newPos);
  link->setPosition(newPos);
}

void GameState::addOccupant(std::shared_ptr<Occupant> occupant, const Position &pos)
{
  board.placeOccupant(occupant, pos);
  occupant->setPosition(pos);
}

void GameState::removeOccupant(std::shared_ptr<Occupant> occupant, const Position &pos)
{
  board.removeOccupant(occupant, pos);
}

void GameState::notifyNextTurn()
{
  // TODO: Implement turn notification
}

void GameState::notifyGameOver()
{
  // TODO: Implement game over notification
}
