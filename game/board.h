#ifndef BOARD_H
#define BOARD_H

#include "cell.h"
#include "occupant.h"
#include "position.h"
#include <vector>
#include <memory>

using namespace std;

class Board
{
  static const int GRID_SIZE = 8;
  vector<vector<unique_ptr<Cell>>> grid;

public:
  Board() : grid{GRID_SIZE, vector<unique_ptr<Cell>>(GRID_SIZE)} {}

  bool isValidPosition(const Position &pos);
  Cell &getCell(Position pos);
  void placeOccupant(shared_ptr<Occupant> occupant, const Position &pos);
  void removeOccupant(shared_ptr<Occupant> occupant, const Position &pos);
};

#endif // BOARD_H