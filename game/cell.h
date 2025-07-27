#ifndef CELL_H
#define CELL_H

#include <vector>
#include <memory>
#include "../utils/position.h"

class Occupant;

class Cell
{
    std::vector<std::shared_ptr<Occupant>> occupants;
    const Position position;
    int type;

public:
    Cell(const Position &position, int type);
    ~Cell();

    std::vector<std::shared_ptr<Occupant>> getOccupants() const;
    Position getPosition() const;
    int getType() const;

    void setType(int type);
    void placeOccupant(std::shared_ptr<Occupant> occupant);
    void removeOccupant(std::shared_ptr<Occupant> occupant);
};

#endif // CELL_H