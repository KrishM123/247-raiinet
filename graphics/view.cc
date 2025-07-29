#include "view.h"

#include <algorithm>
#include <iostream>

#include "../ability/ability.h"
#include "../controller/game_event.h"
#include "../game/board.h"
#include "../game/cell.h"
#include "../game/game_state.h"
#include "../game/link.h"
#include "../game/player.h"

View::View(GameState &gameState, int playerView)
    : playerView(playerView), gameState(gameState),
      gridSize(gameState.getBoard().getGridSize()),
      board(std::vector<std::vector<std::string>>(
          gridSize, std::vector<std::string>(gridSize, "."))),
      linksOnBoard(std::vector<std::vector<std::string>>(
          gridSize, std::vector<std::string>(gridSize, ""))),
      usedAbilities(std::vector<std::vector<std::string>>(
          gameState.getPlayers().size(), std::vector<std::string>(0, ""))),
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
}

void View::notify(GameEvent &event) {
  if (event.getEventType() == EventType::LinkMoved) {
    int oldX = std::stoi(event.getPayload().get("oldX")) - 1;
    int oldY = std::stoi(event.getPayload().get("oldY")) - 1;
    int newX = std::stoi(event.getPayload().get("newX")) - 1;
    int newY = std::stoi(event.getPayload().get("newY")) - 1;
    string oldLink = linksOnBoard[oldX][oldY];
    linksOnBoard[oldX][oldY] = "";
    if (newX >= 0 && newX < gridSize && newY >= 0 && newY < gridSize) {
      linksOnBoard[newX][newY] = oldLink;
    }
  } else if (event.getEventType() == EventType::LinkDownloaded) {
    int x = std::stoi(event.getPayload().get("x")) - 1;
    int y = std::stoi(event.getPayload().get("y")) - 1;
    int player = std::stoi(event.getPayload().get("player")) - 1;
    string type = event.getPayload().get("type");
    if (type == "0") {
      downloadedData[player]++;
    } else if (type == "1") {
      downloadedVirus[player]++;
    }
    if (x >= 0 && x < gridSize && y >= 0 && y < gridSize) {
      linksOnBoard[x][y] = "";
    }
  } else if (event.getEventType() == EventType::AbilityUsed) {
    int player = std::stoi(event.getPayload().get("player"));
    string ability = event.getPayload().get("ability");
    usedAbilities[player].push_back(ability);
    unusedAbilities[player].erase(std::find(unusedAbilities[player].begin(),
                                            unusedAbilities[player].end(),
                                            ability));

  } else if (event.getEventType() == EventType::AbilityPlaced) {
    int x = std::stoi(event.getPayload().get("x")) - 1;
    int y = std::stoi(event.getPayload().get("y")) - 1;
    string ability = event.getPayload().get("ability");
    int player = std::stoi(event.getPayload().get("player"));
    string marker = event.getPayload().get("marker");
    usedAbilities[player].push_back(ability);
    unusedAbilities[player].erase(std::find(unusedAbilities[player].begin(),
                                            unusedAbilities[player].end(),
                                            ability));
    board[x][y] = marker[0];

  } else if (event.getEventType() == EventType::OccupantAdded) {
    int x = std::stoi(event.getPayload().get("x")) - 1;
    int y = std::stoi(event.getPayload().get("y")) - 1;
    string occupant = event.getPayload().get("occupant");
    linksOnBoard[x][y] = occupant[0];

  } else if (event.getEventType() == EventType::OccupantRemoved) {
    int x = std::stoi(event.getPayload().get("x")) - 1;
    int y = std::stoi(event.getPayload().get("y")) - 1;
    linksOnBoard[x][y] = "";
  } else if (event.getEventType() == EventType::BattleTriggered) {
    int x = std::stoi(event.getPayload().get("x")) - 1;
    int y = std::stoi(event.getPayload().get("y")) - 1;
    string winner = event.getPayload().get("winner");
    linksOnBoard[x][y] = winner[0];
  }
}

View::~View() {}
