#ifndef VIEW_H
#define VIEW_H

#include <string>
#include <vector>

class GameEvent;
class GameState;

class View {
protected:
  int playerView;
  GameState &gameState;
  int gridSize;
  std::vector<std::vector<std::string>> board;
  std::vector<std::vector<std::string>> linksOnBoard;
  std::vector<std::vector<std::string>> usedAbilities;
  std::vector<std::vector<std::string>> unusedAbilities;
  std::vector<int> downloadedData;
  std::vector<int> downloadedVirus;

public:
  View(GameState &gameState, int playerView);
  virtual ~View();

  void notify(GameEvent &event);
  virtual void printGame() = 0;
  virtual void printAbilities() = 0;
};

#endif // VIEW_H