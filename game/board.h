#ifndef BOARD_H
#define BOARD_H

#include "cell.h"
#include "occupant.h"
#include "position.h"
#include <vector>
#include <memory>

class Board
{
  std::vector<std::vector<std::unique_ptr<Cell>>> grid;

public:
  bool isValidPosition();
  Cell &getCell(Position pos);
  void placeOccupant(std::shared_ptr<Occupant> occupant, const Position &pos);
  void removeOccupant(std::shared_ptr<Occupant> occupant, const Position &pos);
};

#endif // BOARD_H