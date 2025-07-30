#include "link.h"
#include "../utils/permission.h"
#include "../utils/position.h"
#include <map>
#include <string>

using namespace std;

Link::Link(int type, int strength, Permission perm)
    : Occupant{perm}, type{type}, strength{strength},
      moves{{"up", Position{-1, 0}},
            {"down", Position{1, 0}},
            {"right", Position{0, 1}},
            {"left", Position{0, -1}}},
      isDownloaded{false} {}

// Get the type of the link
int Link::getType() const { return type; }

// Get the strength of the link
int Link::getStrength() const { return strength; }

// Get the moves of the link
const map<string, Position> &Link::getMoves() const { return moves; }

// Get if the link is downloaded
bool Link::getIsDownloaded() const { return isDownloaded; }

// Get the details of the link
string Link::getDetails() const {
  return (type == 0 ? "D" : "V") + to_string(strength);
}

// Get the name of the link
char Link::getName() const { return name; }

// Set the name of the link
void Link::setName(char name) { this->name = name; }

// Set the moves of the link
void Link::setMoves(const map<string, Position> &moves) { this->moves = moves; }

// Set the strength of the link
void Link::setStrength(int strength) {
  this->strength = strength;
}

// Set the type of the link
void Link::setType(int type) { this->type = type; }

// Set if the link is downloaded
void Link::setIsDownloaded(bool isDownloaded) {
  this->isDownloaded = isDownloaded;
}
