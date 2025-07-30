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
  ~Permission();

  shared_ptr<Player> getOwner() const;

  bool viewableBy(const Player &player) const;
  void setVisibleTo(const vector<shared_ptr<Player>> &players);

  void addViewer(shared_ptr<Player> player);

  void setOwner(shared_ptr<Player> newOwner);
};

#endif // PERMISSION_H