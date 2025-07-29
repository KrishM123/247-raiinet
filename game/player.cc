#include "player.h"
#include "game_state.h"
#include "link.h"
#include "occupant.h"
#include "../ability/ability.h"
#include "../utils/permission.h"
#include <sstream>
#include <random>
#include <algorithm>
#include "../ability/barbed_wire.h"
#include "../ability/download.h"
#include "../ability/firewall.h"
#include "../ability/link_boost.h"
#include "../ability/polarize.h"
#include "../ability/scan.h"
#include "../ability/swaplinks.h"
#include "../ability/telescope.h"

using namespace std;

Player::Player(int playerNumber, string links, string abilities) : playerNumber{playerNumber}
{
  // this->abilities = abilities;
}

Player::~Player() = default;

void Player::initLinks(const string &links, Permission perm)
{
  if (!links.empty())
  {
    istringstream linkStream(links);
    string linkType;
    while (linkStream >> linkType)
    {
      int strength = linkType[1] - '0';
      int type = (linkType[0] == 'V') ? 1 : 0;
      this->links.push_back(make_shared<Link>(type, strength, perm));
    }
  }
  else
  {
    vector<shared_ptr<Link>> tempLinks;
    for (int strength = 1; strength <= 4; strength++)
    {
      tempLinks.push_back(make_shared<Link>(0, strength, perm));
      tempLinks.push_back(make_shared<Link>(1, strength, perm));
    }
    random_device rd;
    mt19937 gen(rd());
    shuffle(tempLinks.begin(), tempLinks.end(), gen);

    this->links = tempLinks;
  }
  if (this->playerNumber == 1)
  {
    this->linksMap['a'] = 0;
    this->linksMap['b'] = 1;
    this->linksMap['c'] = 2;
    this->linksMap['d'] = 3;
    this->linksMap['e'] = 4;
    this->linksMap['f'] = 5;
    this->linksMap['g'] = 6;
    this->linksMap['h'] = 7;
  }
  else if (this->playerNumber == 2)
  {
    this->linksMap['A'] = 0;
    this->linksMap['B'] = 1;
    this->linksMap['C'] = 2;
    this->linksMap['D'] = 3;
    this->linksMap['E'] = 4;
    this->linksMap['F'] = 5;
    this->linksMap['G'] = 6;
    this->linksMap['H'] = 7;
  }
}

void Player::initAbilities(const string &abilities, Permission perm, GameState &gameState)
{
  if (!abilities.empty())
  {
    for (char ability : abilities)
    {
      switch (ability)
      {
      case 'B':
        this->abilities.push_back(make_shared<BarbedWire>(perm, gameState));
        break;
      case 'D':
        this->abilities.push_back(make_shared<Download>(perm, gameState));
        break;
      case 'F':
        this->abilities.push_back(make_shared<Firewall>(perm, gameState));
        break;
      case 'L':
        this->abilities.push_back(make_shared<LinkBoost>(perm, gameState));
        break;
      case 'P':
        this->abilities.push_back(make_shared<Polarize>(perm, gameState));
        break;
      case 'A':
        this->abilities.push_back(make_shared<Scan>(perm, gameState));
        break;
      case 'S':
        this->abilities.push_back(make_shared<Swaplinks>(perm, gameState));
        break;
      case 'T':
        this->abilities.push_back(make_shared<Telescope>(perm, gameState));
        break;
      }
    }
  }
  else
  {
    this->abilities.push_back(make_shared<LinkBoost>(perm, gameState));
    this->abilities.push_back(make_shared<Firewall>(perm, gameState));
    this->abilities.push_back(make_shared<Download>(perm, gameState));
    this->abilities.push_back(make_shared<Polarize>(perm, gameState));
    this->abilities.push_back(make_shared<BarbedWire>(perm, gameState));
  }
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

int Player::getPlayerNumber() const
{
  return playerNumber;
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
