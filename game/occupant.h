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
  Occupant(const Position &pos, const Permission &perm) : position{pos}, permission{perm} {}
  virtual ~Occupant();

  Position& getPosition();
  void setPosition(const Position &pos);
};

#endif // OCCUPANT_H