#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "board.h"
#include <memory>
#include <vector>
using namespace std;

class Player;
class Link;
class Occupant;
class Position;

class GameState final {
  Board board;
  std::vector<std::shared_ptr<Player>> players;
  std::shared_ptr<Player> curPlayer;
  bool isGameOver;
  bool obstaclesEnabled;
  bool sideMovesEnabled;

  void notifyNextTurn();
  void notifyGameOver();
  void notifyLinkMoved(std::shared_ptr<Link> link, Position oldPos,
                       Position newPos);
  void notifyLinkDownloaded(std::shared_ptr<Link> link,
                            std::shared_ptr<Player> downloader);

public:
  GameState();
  GameState(int numPlayers, int boardSize, vector<string> links,
            vector<string> abilities, bool obstaclesEnabled,
            bool sideMovesEnabled);
  ~GameState();

  void init();

  bool abilityUsed = false;

  std::vector<std::shared_ptr<Link>> getLinks();
  std::shared_ptr<Link> getLink(char name);
  Board &getBoard();
  std::vector<std::shared_ptr<Player>> getPlayers();
  Player &getCurPlayer() const;

  bool isWon() const;
  std::shared_ptr<Player> getWinner() const;
  void moveLink(std::shared_ptr<Link> link, std::string direction);
  void addOccupant(std::shared_ptr<Occupant> occupant, const Position &pos);
  void removeOccupant(std::shared_ptr<Occupant> occupant, const Position &pos);
  void nextTurn();
  void endGame();
  void downloadLink(std::shared_ptr<Link> link,
                    std::shared_ptr<Player> downloader);
  void notifySideTriggered(char winner, Position &oldPos, Position &newPos);

  void printGame();
};

#endif // GAME_STATE_H
