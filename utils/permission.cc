#include "permission.h"
#include "../game/player.h"

Permission::Permission() : owner{nullptr} {}

Permission::Permission(shared_ptr<Player> owner) : owner{owner} {
  visibleTo.push_back(owner);
}

Permission::~Permission() {}

shared_ptr<Player> Permission::getOwner() const { return owner; }

bool Permission::viewableBy(const Player &player) const {
  for (const auto &viewer : visibleTo) {
    if (viewer.get() == &player) {
      return true;
    }
  }

  return false;
}

void Permission::setVisibleTo(const vector<shared_ptr<Player>> &players) {
  visibleTo = players;
}

void Permission::addViewer(shared_ptr<Player> player) {
  visibleTo.push_back(player);
}

void Permission::setOwner(const Player &newOwner) {
  owner = make_shared<Player>(newOwner);
  visibleTo.push_back(owner);
}