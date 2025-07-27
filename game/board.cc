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
      int type = 0;
      if (i == 0 || i == gridSize - 1)
      {
        type = 1;
        if (j == gridSize / 2 || j == gridSize / 2 - 1)
        {
          type = 3;
        }
      }
      grid[i][j] = std::make_unique<Cell>(Position{i, j}, type);
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
