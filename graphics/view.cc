#include "view.h"

#include <algorithm>

#include "../ability/ability.h"
#include "../controller/game_event.h"
#include "../game/board.h"
#include "../game/cell.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"
#include "../utils/message_queue.h"

View::View(GameState &gameState, int playerView)
    : playerView(playerView),
      gameState(gameState),
      gridSize(gameState.getBoard().getGridSize()),
      board(std::vector<std::vector<std::string>>(
          gridSize, std::vector<std::string>(gridSize, "."))),
      linksOnBoard(std::vector<std::vector<std::string>>(
          gridSize, std::vector<std::string>(gridSize, ""))),
      usedAbilities(std::vector<std::vector<std::string>>(
          0, std::vector<std::string>(0, ""))),
      unusedAbilities(std::vector<std::vector<std::string>>(
          gameState.getPlayers().size(),
          std::vector<std::string>(
              gameState.getCurPlayer().getAbilities().size(), ""))),
      downloadedData(std::vector<int>(gameState.getPlayers().size(), 0)),
      downloadedVirus(std::vector<int>(gameState.getPlayers().size(), 0)) {
  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {
      int type = gameState.getBoard().getCell(Position(i + 1, j + 1)).getType();
      if (type >= 1 && type <= 10) {
        board[i][j] = "S";
      } else if (type >= 11 && type <= 20) {
        board[i][j] = "=";
      } else if (type == -1) {
        board[i][j] = " ";
      }
    }
  }

  for (int i = 0; i < gameState.getPlayers().size(); i++) {
    for (int j = 0; j < gameState.getPlayers()[i]->getLinks().size(); j++) {
      Link &link = *gameState.getPlayers()[i]->getLinks()[j];
      linksOnBoard[link.getPosition().getPosition().first - 1]
                  [link.getPosition().getPosition().second - 1] =
                      link.getName();
    }
  }
  for (int i = 0; i < gameState.getPlayers().size(); i++) {
    for (int j = 0; j < gameState.getPlayers()[i]->getAbilities().size(); j++) {
      Ability &ability = *gameState.getPlayers()[i]->getAbilities()[j];
      unusedAbilities[i][j] = ability.name;
    }
  }
  subscribeToMessageQueue();
}

void View::notify(GameEvent &event) {
  if (event.getEventType() == EventType::LinkMoved) {
    int oldX = std::stoi(event.getPayload().get("oldX"));
    int oldY = std::stoi(event.getPayload().get("oldY"));
    int newX = std::stoi(event.getPayload().get("newX"));
    int newY = std::stoi(event.getPayload().get("newY"));
    string oldLink = linksOnBoard[oldX][oldY];
    linksOnBoard[oldX][oldY] = "";
    linksOnBoard[newX][newY] = oldLink;

  } else if (event.getEventType() == EventType::LinkDownloaded) {
    int x = event.getPayload().get("x")[0] - '0';
    int y = event.getPayload().get("y")[0] - '0';
    int player = event.getPayload().get("player")[0] - '0';
    string type = event.getPayload().get("type");
    if (type == "D") {
      downloadedData[player]++;
    } else if (type == "V") {
      downloadedVirus[player]++;
    }
    linksOnBoard[x][y] = "";

  } else if (event.getEventType() == EventType::AbilityUsed) {
    int player = event.getPayload().get("player")[0] - '0';
    string ability = event.getPayload().get("ability");
    usedAbilities[player].push_back(ability);
    unusedAbilities[player].erase(std::find(unusedAbilities[player].begin(),
                                            unusedAbilities[player].end(),
                                            ability));

  } else if (event.getEventType() == EventType::AbilityPlaced) {
    int x = event.getPayload().get("x")[0] - '0';
    int y = event.getPayload().get("y")[0] - '0';
    string ability = event.getPayload().get("ability");
    int player = event.getPayload().get("player")[0] - '0';
    usedAbilities[player].push_back(ability);
    unusedAbilities[player].erase(std::find(unusedAbilities[player].begin(),
                                            unusedAbilities[player].end(),
                                            ability));
    board[x][y] = ability[0];

  } else if (event.getEventType() == EventType::OccupantAdded) {
    int x = event.getPayload().get("x")[0] - '0';
    int y = event.getPayload().get("y")[0] - '0';
    string occupant = event.getPayload().get("occupant");
    linksOnBoard[x][y] = occupant[0];

  } else if (event.getEventType() == EventType::OccupantRemoved) {
    int x = event.getPayload().get("x")[0] - '0';
    int y = event.getPayload().get("y")[0] - '0';
    linksOnBoard[x][y] = "";
  }
}

View::~View() { unsubscribeFromMessageQueue(); }

void View::subscribeToMessageQueue() {
  MessageQueue::getInstance()->subscribe(this);
}

void View::unsubscribeFromMessageQueue() {
  MessageQueue::getInstance()->unsubscribe(this);
}
