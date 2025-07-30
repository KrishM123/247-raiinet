#include "occupant.h"

// Constructor
Occupant::Occupant(const Permission &perm)
    : position{Position{0, 0}}, permission{perm} {}

// Constructor
Occupant::Occupant(const Position &pos, const Permission &perm)
    : position{pos}, permission{perm} {}

Occupant::~Occupant() = default;

// Get the position of the occupant
Position Occupant::getPosition() { return position; }

// Set the position of the occupant
void Occupant::setPosition(const Position &pos) { position = pos; }
