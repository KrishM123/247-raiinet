#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H

#include "view.h"

class TextDisplay final : public View {
public:
    TextDisplay();
    ~TextDisplay() override;

    void notify(const GameEvent& event) override;
    void printGame(const GameState& gameState) override;
    void printAbilities(const GameState& gameState) override;
};

#endif // TEXT_DISPLAY_H