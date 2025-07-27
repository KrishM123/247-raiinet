#ifndef PERMISSION_H
#define PERMISSION_H

#include <memory>
#include <vector>

using namespace std;

class Player;

class Permission
{
private:
  shared_ptr<Player> owner;
  vector<shared_ptr<Player>> visibleTo;

public:
  Permission();
  Permission(shared_ptr<Player> owner);
  ~Permission();

  shared_ptr<Player> getOwner() const;
  bool viewableBy(const Player &player) const;
  void setOwner(const Player &newOwner);
  void setVisibility(vector<shared_ptr<Player>> &players);
};

#endif // PERMISSION_H