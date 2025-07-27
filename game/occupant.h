#ifndef OCCUPANT_H
#define OCCUPANT_H

#include "../utils/position.h"
#include "../utils/permission.h"

class Occupant
{
public:
  Position position;
  Permission permission;

  Occupant();
  Occupant(const Position &pos, const Permission &perm);
  virtual ~Occupant();

  Position getPosition();
  Permission getPermission();
  void setPosition(const Position &pos);
  void setPermission(const Permission &perm);
};

#endif // OCCUPANT_H