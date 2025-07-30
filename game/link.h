#ifndef LINK_H
#define LINK_H

#include "../utils/permission.h"
#include "../utils/position.h"
#include "occupant.h"
#include <map>
#include <string>

class Link final : public Occupant {
  char name;
  int type;
  // 0: Data, 1: Virus
  int strength;
  std::map<std::string, Position> moves;
  bool isDownloaded;

public:
  Link(int type, int strength, Permission perm);

  int getType() const;
  int getStrength() const;
  const std::map<std::string, Position> &getMoves() const;
  bool getIsDownloaded() const;

  std::string getDetails() const;
  char getName() const;
  void setName(char name);

  void setMoves(const std::map<std::string, Position> &moves);
  void setStrength(int strength);
  void setType(int type);
  void setIsDownloaded(bool isDownloaded);
};

#endif // LINK_H
