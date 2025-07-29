#include "link.h"
#include "../utils/permission.h"
#include "../utils/position.h"
#include <algorithm>
#include <map>
#include <string>

using namespace std;

Link::Link(int type, int strength, Permission perm)
    : Occupant{perm}, type{type}, strength{strength},
      moves{{"up", Position{1, 0}},
            {"down", Position{-1, 0}},
            {"right", Position{0, 1}},
            {"left", Position{0, -1}}},
      isDownloaded{false} {}

Link::~Link() {}

int Link::getType() const { return type; }

int Link::getStrength() const { return strength; }

const map<string, Position> &Link::getMoves() const { return moves; }

bool Link::getIsDownloaded() const { return isDownloaded; }

string Link::getDetails() const {
  return (type == 0 ? "D" : "V") + to_string(strength);
}

char Link::getName() const { return name; }

bool Link::canMove(const string &direction) {
  return moves.find(direction) != moves.end();
}

void Link::setMoves(const map<string, Position> &moves) { this->moves = moves; }

void Link::setStrength(int strength) {
  this->strength = strength;
  notifyLinkStrengthChanged();
}

void Link::setType(int type) { this->type = type; }

void Link::setIsDownloaded(bool isDownloaded) {
  this->isDownloaded = isDownloaded;
}

void Link::notifyLinkDownloaded() {
  // Implementation for link downloaded notification
}

void Link::notifyLinkRevived() {
  // Implementation for link revived notification
}

void Link::notifyLinkStrengthChanged() {
  // Implementation for strength change notification
}
