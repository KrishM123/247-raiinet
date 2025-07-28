#include "view.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../game/board.h"
#include "../game/cell.h"

View::View(GameState& gameState) : 
    gameState(gameState),
    gridSize(gameState.getBoard().getGridSize()),
    board(std::vector<std::vector<std::string>>(gridSize, std::vector<std::string>(gridSize, "."))),
    linksOnBoard(std::vector<std::vector<std::string>>(gridSize, std::vector<std::string>(gridSize, ""))),
    usedAbilities(std::vector<std::vector<std::string>>(0, std::vector<std::string>(0, ""))),
    unusedAbilities(std::vector<std::vector<std::string>>(gameState.getPlayers().size(), std::vector<std::string>(gameState.getCurPlayer().getAbilities().size(), ""))) {
        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                if (gameState.getBoard().getCell(Position(i, j)).getType() == 1) {}
            }
        }
    }


void View::notify(const GameEvent& event) {
    // TODO: Implement
}

void View::printGame() {
    // TODO: Implement
}