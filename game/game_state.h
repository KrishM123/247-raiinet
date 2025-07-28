#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <memory>
#include "board.h"
using namespace std;

class Player;
class Link;
class Occupant;
class Position;

class GameState final
{
    Board board;
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Player> curPlayer;
    bool isGameOver;

    void battleLogic(std::shared_ptr<Link> attacker, Cell &cell);

    void notifyNextTurn();
    void notifyGameOver();

public:
    GameState();
    GameState(int numPlayers, int boardSize, vector<string> links, vector<string> abilities);
    ~GameState();

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
    void downloadLink(std::shared_ptr<Link> link, std::shared_ptr<Player> downloader);
};

#endif // GAME_STATE_H
