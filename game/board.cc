#include "board.h"
#include "../utils/position.h"
#include "cell.h"
#include "link.h"
#include "occupant.h"
#include "player.h"

Board::Board(int gridSize) : gridSize{gridSize} {
  // Create the grid
  grid.resize(gridSize + 2);
  for (int i = 0; i < gridSize + 2; i++) {
    grid[i].resize(gridSize + 2);
    for (int j = 0; j < gridSize + 2; j++) {
      grid[i][j] = make_unique<Cell>(Position{i, j}, -1);
    }
  }
}

// Check if position is on a server cell or out of bounds
bool Board::isValidPosition(const Position &pos, int curPlayer) {
  if (getCell(pos).getType() % 10 == curPlayer % 10 ||
      getCell(pos).getType() == -1) {
    return false;
  }

  // Check if there is a link on the cell
  vector<shared_ptr<Occupant>> occupants = getCell(pos).getOccupants();
  for (auto &occupant : occupants) {
    if (dynamic_pointer_cast<Link>(occupant)) {
      shared_ptr<Link> link = dynamic_pointer_cast<Link>(occupant);
      // If the link is on the same server cell, return false
      if (link->permission.getOwner()->getPlayerNumber() == curPlayer) {
        return false;
      }
    }
  }

  // Check if position is out of bounds
  return pos.getPosition().first >= 0 &&
         pos.getPosition().first <= gridSize + 1 &&
         pos.getPosition().second >= 0 &&
         pos.getPosition().second <= gridSize + 1;
}

// Get the cell at the position
Cell &Board::getCell(const Position &pos) {
  return *grid[pos.getPosition().first][pos.getPosition().second];
}

// Place an occupant on the board
void Board::placeOccupant(shared_ptr<Occupant> occupant, const Position &pos) {
  grid[pos.getPosition().first][pos.getPosition().second]->placeOccupant(
      occupant);
}

// Remove an occupant from the board
void Board::removeOccupant(shared_ptr<Occupant> occupant, const Position &pos) {
  grid[pos.getPosition().first][pos.getPosition().second]->removeOccupant(
      occupant);
}

// Get the grid size
int Board::getGridSize() { return gridSize; }
