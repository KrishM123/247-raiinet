#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <memory>
#include "link.h"
#include "occupant.h"
#include "../ability/ability.h"
#include "game_state.h"

class Player {
    std::string name;
    std::vector<std::shared_ptr<Link>> links;
    std::vector<std::shared_ptr<Ability>> abilities;
    std::vector<std::shared_ptr<Link>> downloadedLinks;

public:
    Player(const std::string& name);
    ~Player();

    std::string getName() const;
    std::vector<std::shared_ptr<Link>> getLinks();
    std::vector<std::shared_ptr<Ability>> getAbilities();
    std::vector<std::shared_ptr<Link>> getDownloadedLinks();

    void setLinks(const std::vector<std::shared_ptr<Link>>& links);
    void setAbilities(const std::vector<std::shared_ptr<Ability>>& abilities);
    void setDownloadedLinks(const std::vector<std::shared_ptr<Link>>& downloadedLinks);
};

#endif // PLAYER_H