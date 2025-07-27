#include "player.h"
#include "link.h"
#include "occupant.h"
#include "../ability/ability.h"
#include "game_state.h"
#include "../utils/permission.h"
#include <sstream>
#include <random>
#include <algorithm>

using namespace std;

Player::Player(int playerNumber, string links, string abilities) : playerNumber{playerNumber}
{
  if (!links.empty())
  {
    istringstream linkStream(links);
    string linkType;
    while (linkStream >> linkType)
    {
      int strength = linkType[1] - '0';
      int type = (linkType[0] == 'V') ? 1 : 0;
      auto perm = make_shared<Permission>(make_shared<Player>(*this));
      this->links.push_back(make_shared<Link>(type, strength, playerNumber, perm));
    }
  }
  else
  {
    vector<shared_ptr<Link>> tempLinks;
    for (int strength = 1; strength <= 4; strength++)
    {
      auto perm = make_shared<Permission>(make_shared<Player>(*this));
      tempLinks.push_back(make_shared<Link>(0, strength, playerNumber, perm));

      perm = make_shared<Permission>(make_shared<Player>(*this));
      tempLinks.push_back(make_shared<Link>(1, strength, playerNumber, perm));
    }
    random_device rd;
    mt19937 gen(rd());
    shuffle(tempLinks.begin(), tempLinks.end(), gen);

    this->links = tempLinks;
  }
  // this->abilities = abilities;
}

Player::~Player()
{
}

vector<shared_ptr<Link>> Player::getLinks()
{
  return links;
}

vector<shared_ptr<Ability>> Player::getAbilities()
{
  return abilities;
}

vector<shared_ptr<Link>> Player::getDownloadedLinks()
{
  return downloadedLinks;
}

pair<int, int> Player::getScore() const
{
  int dataCount = 0;
  int virusCount = 0;

  for (const auto &link : downloadedLinks)
  {
    if (link->getType() == 0)
    {
      dataCount++;
    }
    else
    {
      virusCount++;
    }
  }

  return make_pair(dataCount, virusCount);
}

void Player::setLinks(const vector<shared_ptr<Link>> &links)
{
  this->links = links;
}

void Player::setAbilities(const vector<shared_ptr<Ability>> &abilities)
{
  this->abilities = abilities;
}

void Player::setDownloadedLinks(const vector<shared_ptr<Link>> &downloadedLinks)
{
  this->downloadedLinks = downloadedLinks;
}
