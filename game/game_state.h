#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <memory>
#include "board.h"

class Player;
class Link;
class Occupant;
class Position;

class GameState final
{
    Board board;
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Player> curPlayer;

    void notifyNextTurn();
    void notifyGameOver();

public:
    GameState(int numPlayers, int boardSize);
    ~GameState();

    std::vector<std::shared_ptr<Link>> getLinks();
    Board &getBoard();
    std::vector<std::shared_ptr<Player>> getPlayers();
    Player &getCurPlayer() const;
    bool isWon() const;
    std::shared_ptr<Player> getWinner() const;
    void moveLink(std::shared_ptr<Link> link, const Position &newPos);
    void addOccupant(std::shared_ptr<Occupant> occupant, const Position &pos);
    void removeOccupant(std::shared_ptr<Occupant> occupant, const Position &pos);
};

#endif // GAME_STATE_H
