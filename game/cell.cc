#include "cell.h"
#include "occupant.h"
#include "../utils/position.h"
#include <algorithm>

Cell::Cell(const Position &position, int type) : position{position}, type{type} {}

Cell::~Cell() {}

std::vector<std::shared_ptr<Occupant>> Cell::getOccupants() const
{
  return occupants;
}

Position Cell::getPosition() const
{
  return position;
}

int Cell::getType() const
{
  return type;
}

void Cell::setType(int type)
{
  this->type = type;
}

void Cell::placeOccupant(std::shared_ptr<Occupant> occupant)
{
  occupants.push_back(occupant);
}

void Cell::removeOccupant(std::shared_ptr<Occupant> occupant)
{
  occupants.erase(std::remove(occupants.begin(), occupants.end(), occupant), occupants.end());
}
