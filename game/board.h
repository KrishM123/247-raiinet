#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>

using namespace std;

class Cell;
class Occupant;
class Position;

class Board final
{
  int gridSize;
  vector<vector<unique_ptr<Cell>>> grid;

public:
  Board(int gridSize);
  ~Board();

  bool isValidPosition(const Position &pos, int curPlayer);
  Cell &getCell(const Position &pos);
  void placeOccupant(shared_ptr<Occupant> occupant, const Position &pos);
  void removeOccupant(shared_ptr<Occupant> occupant, const Position &pos);
  int getGridSize();
};

#endif // BOARD_H