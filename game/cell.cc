#include "cell.h"
#include "occupant.h"
#include "../utils/position.h"
#include "../ability/trigger.h"
#include "../utils/payload.h"
#include <string>
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
  occupant->setPosition(position);
  for (auto &occupant : occupants)
  {
    if (dynamic_pointer_cast<Trigger>(occupant))
    {
      shared_ptr<Trigger> trigger = dynamic_pointer_cast<Trigger>(occupant);
      trigger->trigger();
    }
  }
}

void Cell::removeOccupant(std::shared_ptr<Occupant> occupant)
{
  occupants.erase(std::remove(occupants.begin(), occupants.end(), occupant), occupants.end());
}
