#ifndef PLAYER_H
#define PLAYER_H

#include "../ability/ability.h"
#include "game_state.h"
#include "link.h"
#include "occupant.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class Player {
  int playerNumber;
  std::vector<std::shared_ptr<Link>> links;
  std::vector<std::shared_ptr<Ability>> abilities;
  std::vector<std::shared_ptr<Link>> downloadedLinks;

public:
  std::map<char, int> linksMap;

  Player(int playerNumber, string links, string abilities);

  void initLinks(const std::string &links, Permission perm);
  void initAbilities(const std::string &abilities, Permission perm,
                     GameState &gameState);

  std::vector<std::shared_ptr<Link>> getLinks();
  std::vector<std::shared_ptr<Ability>> getAbilities();
  std::vector<std::shared_ptr<Link>> getDownloadedLinks();
  std::pair<int, int> getScore() const;
  int getPlayerNumber() const;

  void setDownloadedLinks(const std::vector<std::shared_ptr<Link>> &downloadedLinks);
};

#endif // PLAYER_H
