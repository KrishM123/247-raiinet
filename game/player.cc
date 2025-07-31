#include "player.h"
#include "../ability/ability.h"
#include "../ability/barbed_wire.h"
#include "../ability/download.h"
#include "../ability/firewall.h"
#include "../ability/link_boost.h"
#include "../ability/polarize.h"
#include "../ability/scan.h"
#include "../ability/swaplinks.h"
#include "../ability/telescope.h"
#include "../utils/permission.h"
#include "game_state.h"
#include "link.h"
#include <algorithm>
#include <random>
#include <sstream>

using namespace std;

// Constructor
Player::Player(int playerNumber, string links, string abilities)
    : playerNumber{playerNumber} {}

// Initialize the links of the player
void Player::initLinks(const string &links, Permission perm) {
  // If the links are not empty, parse the links
  if (!links.empty()) {
    istringstream linkStream(links);
    string linkType;
    while (linkStream >> linkType) {
      int strength = linkType[1] - '0';
      int type = (linkType[0] == 'V') ? 1 : 0;
      this->links.push_back(make_shared<Link>(type, strength, perm));
    }
  } else {
    // If the links are empty, generate 8 random links
    vector<shared_ptr<Link>> tempLinks;
    for (int strength = 1; strength <= 4; strength++) {
      tempLinks.push_back(make_shared<Link>(0, strength, perm));
      tempLinks.push_back(make_shared<Link>(1, strength, perm));
    }
    random_device rd;
    mt19937 gen(rd());
    shuffle(tempLinks.begin(), tempLinks.end(), gen);

    this->links = tempLinks;
  }
  // If the player is player 1
  if (this->playerNumber == 1) {
    for (int i = 0; i < this->links.size(); i++) {
      this->links[i]->setName('a' + i);
    }
    // Set the links map
    this->linksMap['a'] = 0;
    this->linksMap['b'] = 1;
    this->linksMap['c'] = 2;
    this->linksMap['d'] = 3;
    this->linksMap['e'] = 4;
    this->linksMap['f'] = 5;
    this->linksMap['g'] = 6;
    this->linksMap['h'] = 7;
    // If the player is player 2
  } else if (this->playerNumber == 2) {
    for (int i = 0; i < this->links.size(); i++) {
      this->links[i]->setName('A' + i);
    }
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

// Initialize the abilities of the player
void Player::initAbilities(const string &abilities, Permission perm,
                           GameState &gameState) {
  if (!abilities.empty()) {
    for (char ability : abilities) {
      switch (ability) {
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
  } else {
    this->abilities.push_back(make_shared<LinkBoost>(perm, gameState));
    this->abilities.push_back(make_shared<Firewall>(perm, gameState));
    this->abilities.push_back(make_shared<Download>(perm, gameState));
    this->abilities.push_back(make_shared<Polarize>(perm, gameState));
    this->abilities.push_back(make_shared<BarbedWire>(perm, gameState));
  }
}

// Get the links of the player
vector<shared_ptr<Link>> Player::getLinks() { return links; }

// Get the abilities of the player
vector<shared_ptr<Ability>> Player::getAbilities() { return abilities; }

// Get the downloaded links of the player
vector<shared_ptr<Link>> Player::getDownloadedLinks() {
  return downloadedLinks;
}

// Get the player number
int Player::getPlayerNumber() const { return playerNumber; }

// Get the score of the player
pair<int, int> Player::getScore() const {
  int dataCount = 0;
  int virusCount = 0;

  for (const auto &link : downloadedLinks) {
    if (link->getType() == 0) {
      dataCount++;
    } else {
      virusCount++;
    }
  }

  return make_pair(dataCount, virusCount);
}

// Set the downloaded links of the player
void Player::setDownloadedLinks(
    const vector<shared_ptr<Link>> &downloadedLinks) {
  this->downloadedLinks = downloadedLinks;
}

int Player::getUnusedAbilities() const {
  int unusedAbilities = 0;
  for (const auto &ability : abilities) {
    if (!ability->used) {
      unusedAbilities++;
    }
  }
  return unusedAbilities;
}
