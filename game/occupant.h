#ifndef OCCUPANT_H
#define OCCUPANT_H

#include "position.h"

class Occupant
{
public:
  Position position;
  Permission permission;

  Occupant(const Position &pos, const Permission &perm) : position{pos}, permission{perm} {}

  Position &getPosition();
  void setPosition(const Position &pos);
};

#endif // OCCUPANT_H