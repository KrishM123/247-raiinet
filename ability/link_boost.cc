#include "../ability/link_boost.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../game/link.h"
#include "../utils/payload.h"
#include "../utils/position.h"
#include <string>
#include <sstream>
#include <map>

using namespace std;

// Helper:
// Parse link character into player and link indices
pair<int, int> getBoostTargetIndices(char linkId)
{
    if (linkId >= 'a' && linkId <= 'h')
    {
        return {0, linkId - 'a'}; // Player 0, link index
    }
    if (linkId >= 'A' && linkId <= 'H')
    {
        return {1, linkId - 'A'}; // Player 1, link index
    }
    return {-1, -1};
}

LinkBoost::LinkBoost(Permission &permission, GameState &gameState) : Ability("L", permission, gameState) {}

LinkBoost::~LinkBoost() {}

void LinkBoost::execute(const Payload &payload)
{
    // Input: string (single char) representing a link
    // Example: "a"

    string args = payload.get("args");
    stringstream ss(args);
    string linkIdStr;

    ss >> linkIdStr;

    if (linkIdStr.length() != 1 || !ss.eof())
    {
        return;
    }
    char linkId = linkIdStr[0];

    pair<int, int> target = getBoostTargetIndices(linkId);
    int targetPlayerIndex = target.first;
    int targetLinkIndex = target.second;

    if (targetPlayerIndex == -1)
        return;

    // A player can only boost their own link
    if (targetPlayerIndex != gameState.getCurPlayer().getPlayerNumber())
    {
        return;
    }

    shared_ptr<Player> targetPlayer = gameState.getPlayers()[targetPlayerIndex];
    vector<shared_ptr<Link>> targetPlayerLinks = targetPlayer->getLinks();

    if (targetLinkIndex >= targetPlayerLinks.size())
        return;
    shared_ptr<Link> targetLink = targetPlayerLinks[targetLinkIndex];

    // Apply boost
    map<string, Position> currentMoves = targetLink->getMoves();
    map<string, Position> boostedMoves;

    for (const auto &move : currentMoves)
    {
        Position originalDelta = move.second;
        pair<int, int> deltaCoords = originalDelta.getPosition();

        int newRow = deltaCoords.first;
        if (newRow > 0)
        {
            newRow++;
        }
        else if (newRow < 0)
        {
            newRow--;
        }

        int newCol = deltaCoords.second;
        if (newCol > 0)
        {
            newCol++;
        }
        else if (newCol < 0)
        {
            newCol--;
        }

        boostedMoves[move.first] = Position{newRow, newCol};
    }

    targetLink->setMoves(boostedMoves);

    notifyAbilityUsed();
}