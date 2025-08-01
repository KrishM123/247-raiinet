#ifndef PERMISSION_H
#define PERMISSION_H

#include <memory>
#include <vector>

using namespace std;

class Player;

class Permission {
private:
  weak_ptr<Player> owner;
  vector<weak_ptr<Player>> visibleTo;

public:
  Permission();
  Permission(shared_ptr<Player> owner);

  shared_ptr<Player> getOwner() const;

  bool viewableBy(const Player &player) const;
  void setVisibleTo(const vector<shared_ptr<Player>> &players);
  vector<weak_ptr<Player>> getVisibleTo() const;

  void addViewer(shared_ptr<Player> player);
};

#endif // PERMISSION_H
