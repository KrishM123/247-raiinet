#include "permission.h"
#include "../game/player.h"

Permission::Permission() {}

Permission::Permission(shared_ptr<Player> owner) : owner{owner} {
  visibleTo.push_back(owner);
}

shared_ptr<Player> Permission::getOwner() const { return owner.lock(); }

bool Permission::viewableBy(const Player &player) const {
  // Check if the player is in the visibleTo list
  for (const auto &viewer : visibleTo) {
    if ((*viewer.lock()).getPlayerNumber() == player.getPlayerNumber()) {
      return true;
    }
  }

  return false;
}

void Permission::setVisibleTo(const vector<shared_ptr<Player>> &players) {
  // Clear the visibleTo list
  visibleTo.clear();
  for (const auto &player : players) {
    visibleTo.push_back(player);
  }
}

void Permission::addViewer(shared_ptr<Player> player) {
  // Add the player to the visibleTo list
  visibleTo.push_back(player);
}
