#include "board.h"
#include "cell.h"
#include "occupant.h"
#include "../utils/position.h"

Board::Board(int gridSize) : gridSize{gridSize}
{
  grid.resize(gridSize + 2);
  for (int i = 0; i < gridSize + 2; i++)
  {
    grid[i].resize(gridSize + 2);
    for (int j = 0; j < gridSize + 2; j++)
    {
      if (j == 0 || j == gridSize + 1)
      {
        grid[i][j] = std::make_unique<Cell>(Position{i, j}, -1);
        continue;
      }
      if (i == 0)
      {
        grid[i][j] = std::make_unique<Cell>(Position{i, j}, 11);
      }
      else if (i == gridSize + 1)
      {
        grid[i][j] = std::make_unique<Cell>(Position{i, j}, 12);
      }
      else
      {
        int type = 0;
        if (i == 1 && (j == gridSize / 2 || j == gridSize / 2 - 1))
        {
          type = 1;
        }
        else if (i == gridSize && (j == gridSize / 2 || j == gridSize / 2 - 1))
        {
          type = 2;
        }
        grid[i][j] = std::make_unique<Cell>(Position{i, j}, type);
      }
    }
  }
}

Board::~Board() {};

bool Board::isValidPosition(const Position &pos, int curPlayer)
{
  // Check if position is on a server cell or out of bounds
  if (getCell(pos).getType() == curPlayer % 10 || getCell(pos).getType() == -1)
  {
    return false;
  }

  return pos.getPosition().first >= 1 && pos.getPosition().first <= gridSize && pos.getPosition().second >= 0 && pos.getPosition().second < gridSize;
}

Cell &Board::getCell(const Position &pos)
{
  return *grid[pos.getPosition().first][pos.getPosition().second];
}

void Board::placeOccupant(shared_ptr<Occupant> occupant, const Position &pos)
{
  grid[pos.getPosition().first][pos.getPosition().second]->placeOccupant(occupant);
  occupant->setPosition(pos);
}

void Board::removeOccupant(shared_ptr<Occupant> occupant, const Position &pos)
{
  grid[pos.getPosition().first][pos.getPosition().second]->removeOccupant(occupant);
}

int Board::getGridSize()
{
  return gridSize;
}
