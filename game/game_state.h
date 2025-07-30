#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "board.h"
#include <memory>

using namespace std;

class Player;
class Link;
class Occupant;
class Position;

class GameState final {
  Board board;
  vector<shared_ptr<Player>> players;
  shared_ptr<Player> curPlayer;
  bool isGameOver;
  bool obstaclesEnabled;
  bool sideMovesEnabled;

  void notifyLinkMoved(shared_ptr<Link> link, Position oldPos, Position newPos);
  void notifyLinkDownloaded(shared_ptr<Link> link,
                            shared_ptr<Player> downloader);
  void notifySideTriggered(char winner, Position &oldPos, Position &newPos);

public:
  GameState(int numPlayers, int boardSize, vector<string> links,
            vector<string> abilities, bool obstaclesEnabled,
            bool sideMovesEnabled);

  void init();

  bool abilityUsed = false;

  std::shared_ptr<Link> getLink(char name);
  Board &getBoard();
  std::vector<std::shared_ptr<Player>> getPlayers();
  Player &getCurPlayer() const;

  void moveLink(std::shared_ptr<Link> link, std::string direction);
  void addOccupant(std::shared_ptr<Occupant> occupant, const Position &pos);
  void removeOccupant(std::shared_ptr<Occupant> occupant, const Position &pos);

  bool isWon() const;
  std::shared_ptr<Player> getWinner() const;
  void nextTurn();
  void endGame();
  void downloadLink(std::shared_ptr<Link> link,
                    std::shared_ptr<Player> downloader);
};

#endif // GAME_STATE_H
