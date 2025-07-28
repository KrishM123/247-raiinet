#ifndef VIEW_H
#define VIEW_H

#include <vector>
#include <string>

class GameEvent;
class GameState;

class View {
protected:
    const GameState& gameState;
    std::vector<std::vector<std::string>> board;
    std::vector<std::vector<std::string>> linksOnBoard;
    std::vector<std::vector<std::string>> usedAbilities;
    std::vector<std::vector<std::string>> unusedAbilities;

public:
    View(const GameState& gameState);
    virtual ~View() = default;

    virtual void notify(const GameEvent& event) = 0;
    virtual void printGame() = 0;
    virtual void printAbilities() = 0;
};

#endif // VIEW_H