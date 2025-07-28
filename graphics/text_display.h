#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H

#include "view.h"

class GameState;
class GameEvent;

class TextDisplay final : public View {
public:
    TextDisplay(const GameState& gameState);
    ~TextDisplay() override;

    void notify(const GameEvent& event) override;
    void printGame() override;
    void printAbilities() override;
};

#endif // TEXT_DISPLAY_H