#include "occupant.h"

Occupant::Occupant(const Permission &perm)
    : position{Position{0, 0}}, permission{perm} {}

Occupant::Occupant(const Position &pos, const Permission &perm)
    : position{pos}, permission{perm} {}

Occupant::~Occupant() = default;

Position Occupant::getPosition() { return position; }

void Occupant::setPosition(const Position &pos) { position = pos; }
