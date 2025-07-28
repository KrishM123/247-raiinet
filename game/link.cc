#include "link.h"
using namespace std;

Link::Link(int type, int strength, const Permission &perm) : Occupant{perm}, type{type}, strength{strength}, moves{{Position{1, 0}, Position{-1, 0}, Position{0, 1}, Position{0, -1}}}, isDownloaded{false} {}

Link::~Link() {}

int Link::getType() const
{
  return type;
}

int Link::getStrength() const
{
  return strength;
}

const vector<Position> &Link::getMoves() const
{
  return moves;
}

bool Link::getIsDownloaded() const
{
  return isDownloaded;
}

bool Link::canMove(const Position &pos) const
{
  return find(moves.begin(), moves.end(), pos) != moves.end();
}

void Link::setMoves(const vector<Position> &moves)
{
  this->moves = moves;
}

void Link::setStrength(int strength)
{
  this->strength = strength;
  notifyLinkStrengthChanged();
}

void Link::setType(int type)
{
  this->type = type;
}

void Link::setIsDownloaded(bool isDownloaded)
{
  this->isDownloaded = isDownloaded;
}

void Link::notifyLinkDownloaded()
{
  // Implementation for link downloaded notification
}

void Link::notifyLinkRevived()
{
  // Implementation for link revived notification
}

void Link::notifyLinkStrengthChanged()
{
  // Implementation for strength change notification
}
