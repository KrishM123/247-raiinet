#include "cell.h"
#include "../ability/trigger.h"
#include "../utils/position.h"
#include "occupant.h"
#include <algorithm>

// Constructor
Cell::Cell(const Position &position, int type)
    : position{position}, type{type} {}

// Get the occupants of the cell
std::vector<std::shared_ptr<Occupant>> Cell::getOccupants() const {
  return occupants;
}

// Get the position of the cell
Position Cell::getPosition() const { return position; }

// Get the type of the cell
int Cell::getType() const { return type; }

// Set the type of the cell
void Cell::setType(int type) { this->type = type; }

// Place an occupant on the cell
void Cell::placeOccupant(std::shared_ptr<Occupant> occupant) {
  occupants.push_back(occupant);
  occupant->setPosition(position);
  // Trigger the triggers of the occupants
  for (auto &occupant : occupants) {
    if (dynamic_pointer_cast<Trigger>(occupant)) {
      shared_ptr<Trigger> trigger = dynamic_pointer_cast<Trigger>(occupant);
      trigger->trigger();
    }
  }
}

// Remove an occupant from the cell
void Cell::removeOccupant(std::shared_ptr<Occupant> occupant) {
  occupants.erase(std::remove(occupants.begin(), occupants.end(), occupant),
                  occupants.end());
}
