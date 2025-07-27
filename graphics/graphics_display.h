#ifndef GRAPHICS_DISPLAY_H
#define GRAPHICS_DISPLAY_H

#include "view.h"
#include "x_window.h"

class GraphicsDisplay final : public View {
public:
    GraphicsDisplay();
    ~GraphicsDisplay() override;

    void notify(const GameEvent& event) override;
    void printGame(const GameState& gameState) override;
    void printAbilities(const GameState& gameState) override;

private:
    XWindow window;
};

#endif // GRAPHICS_DISPLAY_H