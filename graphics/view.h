#ifndef VIEW_H
#define VIEW_H

#include <string>
#include <vector>

class GameEvent;
class GameState;
class Player;
class Payload;

class View {
protected:
  int playerView;
  GameState &gameState;
  int gridSize;
  std::vector<std::vector<std::string>> oldBoard;
  std::vector<std::vector<std::string>> board;
  std::vector<std::vector<std::string>> linksOnBoard;
  std::vector<int> downloadedData;
  std::vector<int> downloadedVirus;

public:
  View(GameState &gameState, int playerView);
  void notify(GameEvent &event);
  virtual void printGame() = 0;
  void printAbilities(Player &playerDisplay);
  void storeOldboard();
  Payload getDiff();
};

#endif // VIEW_H
