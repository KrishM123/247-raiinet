#include "serverTrigger.h"
#include "../../utils/payload.h"
#include "../../game/game_state.h"
#include "../../game/player.h"
#include <memory>

using namespace std;

ServerTrigger::ServerTrigger(GameState &gameState, const Position &pos, const Permission &perm) : Trigger{gameState, pos, perm}
{
  triggerAction = [this](const Payload &payload)
  {
    handleServerInteraction(payload);
  };
}

ServerTrigger::~ServerTrigger() {}

void ServerTrigger::handleServerInteraction(const Payload &payload)
{
  shared_ptr<Link> link = gameState.getLink(payload.get("link")[0]);
  shared_ptr<Player> serverOwner = permission.getOwner();
  shared_ptr<Player> linkOwner = link->permission.getOwner();

  if (serverOwner != linkOwner)
  {
    gameState.downloadLink(link, serverOwner);
  }
}