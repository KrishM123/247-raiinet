#include "board.h"

#include <iostream>

#include "../utils/position.h"
#include "cell.h"
#include "link.h"
#include "occupant.h"
#include "player.h"

Board::Board(int gridSize) : gridSize{gridSize} {
  grid.resize(gridSize + 2);
  for (int i = 0; i < gridSize + 2; i++) {
    grid[i].resize(gridSize + 2);
    for (int j = 0; j < gridSize + 2; j++) {
      grid[i][j] = std::make_unique<Cell>(Position{i, j}, -1);
    }
  }
}

Board::~Board() {};

bool Board::isValidPosition(const Position &pos, int curPlayer) {
  // Check if position is on a server cell or out of bounds
  if (getCell(pos).getType() % 10 == curPlayer % 10 ||
      getCell(pos).getType() == -1) {
    return false;
  }

  vector<shared_ptr<Occupant>> occupants = getCell(pos).getOccupants();
  for (auto &occupant : occupants) {
    if (dynamic_pointer_cast<Link>(occupant)) {
      shared_ptr<Link> link = dynamic_pointer_cast<Link>(occupant);
      if (link->permission.getOwner()->getPlayerNumber() == curPlayer) {
        return false;
      }
    }
  }

  return pos.getPosition().first >= 0 &&
         pos.getPosition().first <= gridSize + 1 &&
         pos.getPosition().second >= 0 &&
         pos.getPosition().second <= gridSize + 1;
}

Cell &Board::getCell(const Position &pos) {
  return *grid[pos.getPosition().first][pos.getPosition().second];
}

void Board::placeOccupant(shared_ptr<Occupant> occupant, const Position &pos) {
  grid[pos.getPosition().first][pos.getPosition().second]->placeOccupant(
      occupant);
}

void Board::removeOccupant(shared_ptr<Occupant> occupant, const Position &pos) {
  grid[pos.getPosition().first][pos.getPosition().second]->removeOccupant(
      occupant);
}

int Board::getGridSize() { return gridSize; }