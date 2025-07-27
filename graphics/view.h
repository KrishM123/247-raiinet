#ifndef VIEW_H
#define VIEW_H

class GameEvent;
class GameState;

class View {
public:
    View();
    virtual ~View() = default;

    virtual void notify(const GameEvent& event) = 0;
    virtual void printGame(const GameState& gameState) = 0;
    virtual void printAbilities(const GameState& gameState) = 0;
};

#endif // VIEW_H