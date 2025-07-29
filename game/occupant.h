#ifndef OCCUPANT_H
#define OCCUPANT_H

#include "../utils/permission.h"
#include "../utils/position.h"

class Occupant {
public:
  Position position;
  Permission permission;

  Occupant(const Permission &perm);
  Occupant(const Position &pos, const Permission &perm);
  virtual ~Occupant() = 0;

  Position getPosition();
  void setPosition(const Position &pos);
};

#endif // OCCUPANT_H