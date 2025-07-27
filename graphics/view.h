#ifndef VIEW_H
#define VIEW_H

// Forward-declarations
class GameEvent;
class GameState;

class View {
public:
    View();
    virtual ~View() = default;

    // Pure virtual functions to make this an interface
    virtual void notify(const GameEvent& event) = 0;
    virtual void printGame(const GameState& gameState) = 0;
    virtual void printAbilities(const GameState& gameState) = 0;
};

#endif // VIEW_H