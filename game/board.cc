#include "board.h"
#include "../utils/position.h"
#include "cell.h"
#include "occupant.h"

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
  if (getCell(pos).getType() == curPlayer % 10 ||
      getCell(pos).getType() == -1) {
    return false;
  }

  return pos.getPosition().first >= 1 && pos.getPosition().first <= gridSize &&
         pos.getPosition().second >= 0 && pos.getPosition().second < gridSize;
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
