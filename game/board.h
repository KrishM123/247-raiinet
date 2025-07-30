#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <vector>
#include "cell.h"

using namespace std;

class Occupant;
class Position;

class Board final {
  int gridSize;
  vector<vector<unique_ptr<Cell>>> grid;

public:
  Board(int gridSize);
  Board(const Board &other) = delete;
  Board &operator=(const Board &other) = delete;

  bool isValidPosition(const Position &pos, int curPlayer);
  Cell &getCell(const Position &pos);
  void placeOccupant(shared_ptr<Occupant> occupant, const Position &pos);
  void removeOccupant(shared_ptr<Occupant> occupant, const Position &pos);
  int getGridSize();
};

#endif // BOARD_H
