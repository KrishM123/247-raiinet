#include "permission.h"
#include "../game/player.h"

Permission::Permission() : owner{} {}

Permission::Permission(shared_ptr<Player> owner) : owner{owner} {
  visibleTo.push_back(owner);
}

Permission::~Permission() {}

shared_ptr<Player> Permission::getOwner() const { return owner.lock(); }

bool Permission::viewableBy(const Player &player) const {
  for (const auto &viewer : visibleTo) {
    if ((*viewer.lock()).getPlayerNumber() == player.getPlayerNumber()) {
      return true;
    }
  }

  return false;
}

void Permission::setVisibleTo(const vector<shared_ptr<Player>> &players) {
  visibleTo.clear();
  for (const auto &player : players) {
    visibleTo.push_back(player);
  }
}

void Permission::addViewer(shared_ptr<Player> player) {
  visibleTo.push_back(player);
}

void Permission::setOwner(shared_ptr<Player> newOwner) {
    bool already_visible = false;
    for (const auto& viewer : visibleTo) {
        if ((*viewer.lock()).getPlayerNumber() == newOwner->getPlayerNumber()) {
            already_visible = true;
            break;
        }
    }
    if (!already_visible) {
        visibleTo.push_back(owner.lock());
    }
}