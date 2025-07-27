#include "board.h"
#include "cell.h"
#include "occupant.h"
#include "../utils/position.h"

Board::Board(int gridSize) : gridSize{gridSize}
{
  grid.resize(gridSize);
  for (int i = 0; i < gridSize; i++)
  {
    grid[i].resize(gridSize);
    for (int j = 0; j < gridSize; j++)
    {
      grid[i][j] = make_unique<Cell>();
    }
  }
}

bool Board::isValidPosition(const Position &pos)
{
  return pos.getPosition().first >= 0 && pos.getPosition().first < gridSize && pos.getPosition().second >= 0 && pos.getPosition().second < gridSize;
}

Cell &Board::getCell(const Position &pos)
{
  return *grid[pos.getPosition().first][pos.getPosition().second];
}

void Board::placeOccupant(shared_ptr<Occupant> occupant, const Position &pos)
{
  if (isValidPosition(pos))
  {
    grid[pos.getPosition().first][pos.getPosition().second]->placeOccupant(occupant);
  }
}

void Board::removeOccupant(shared_ptr<Occupant> occupant, const Position &pos)
{
  if (isValidPosition(pos))
  {
    grid[pos.getPosition().first][pos.getPosition().second]->removeOccupant(occupant);
  }
}
