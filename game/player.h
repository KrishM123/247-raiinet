#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "link.h"
#include "occupant.h"
#include "../ability/ability.h"
#include "game_state.h"

class Player
{
    int playerNumber;
    std::vector<std::shared_ptr<Link>> links;
    std::vector<std::shared_ptr<Ability>> abilities;
    std::vector<std::shared_ptr<Link>> downloadedLinks;
public:
    std::map<char, int> linksMap;
    
    Player(int playerNumber, string links, string abilities);
    ~Player();

    void initLinks(const std::string &links, Permission perm);

    std::vector<std::shared_ptr<Link>> getLinks();
    std::vector<std::shared_ptr<Ability>> getAbilities();
    std::vector<std::shared_ptr<Link>> getDownloadedLinks();
    std::pair<int, int> getScore() const;
    int getPlayerNumber() const;

    void setLinks(const std::vector<std::shared_ptr<Link>> &links);
    void setAbilities(const std::vector<std::shared_ptr<Ability>> &abilities);
    void setDownloadedLinks(const std::vector<std::shared_ptr<Link>> &downloadedLinks);
};

#endif // PLAYER_H