#ifndef VIEW_H
#define VIEW_H

#include <vector>
#include <string>

class GameEvent;
class GameState;

class View {
protected:
    GameState& gameState;
    int gridSize;
    std::vector<std::vector<std::string>> board;
    std::vector<std::vector<std::string>> linksOnBoard;
    std::vector<std::vector<std::string>> usedAbilities;
    std::vector<std::vector<std::string>> unusedAbilities;
    std::vector<int> downloadedData;
    std::vector<int> downloadedVirus;

public:
    View(GameState& gameState);
    virtual ~View() = default;

    void notify(const GameEvent& event);
    virtual void printGame() = 0;
    virtual void printAbilities() = 0;
};

#endif // VIEW_H