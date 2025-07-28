#ifndef GRAPHICS_DISPLAY_H
#define GRAPHICS_DISPLAY_H

#include "view.h"
#include "x_window.h"

class GameState;
class GameEvent;

class GraphicsDisplay final : public View {
public:
    GraphicsDisplay(GameState& gameState);
    ~GraphicsDisplay() override;

    void notify(const GameEvent& event) override;
    void printGame() override;
    void printAbilities() override;

private:
    XWindow window;
};

#endif // GRAPHICS_DISPLAY_H