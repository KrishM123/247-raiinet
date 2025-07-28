#include "view.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "../game/board.h"
#include "../game/cell.h"
#include "../game/link.h"
#include "../ability/ability.h"
#include "../controller/event_types.h"

View::View(GameState& gameState) : 
    gameState(gameState),
    gridSize(gameState.getBoard().getGridSize()),
    board(std::vector<std::vector<std::string>>(gridSize, std::vector<std::string>(gridSize, "."))),
    linksOnBoard(std::vector<std::vector<std::string>>(gridSize, std::vector<std::string>(gridSize, ""))),
    usedAbilities(std::vector<std::vector<std::string>>(0, std::vector<std::string>(0, ""))),
    unusedAbilities(std::vector<std::vector<std::string>>(gameState.getPlayers().size(), std::vector<std::string>(gameState.getCurPlayer().getAbilities().size(), ""))) {
        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                int type = gameState.getBoard().getCell(Position(i, j)).getType();
                if (type >= 1 && type <= 10) {
                    linksOnBoard[i][j] = "S";
                } else if (type >= 11 && type <= 20) {
                    linksOnBoard[i][j] = "=";
                } else if (type == -1) {
                    linksOnBoard[i][j] = " ";
                }
            }
        }
        for (int i = 0; i < gameState.getPlayers().size(); i++) {
            for (int j = 0; j < gameState.getPlayers()[i]->getLinks().size(); j++) {
                Link& link = *gameState.getPlayers()[i]->getLinks()[j];
                linksOnBoard[link.getPosition().getPosition().first][link.getPosition().getPosition().second] = j - 'a' + 1;
            }
        }
        for (int i = 0; i < gameState.getPlayers().size(); i++) {
            for (int j = 0; j < gameState.getPlayers()[i]->getAbilities().size(); j++) {
                Ability& ability = *gameState.getPlayers()[i]->getAbilities()[j];
                unusedAbilities[i][j] = ability.name;
            }
        }
    }


void View::notify(const GameEvent& event) {\
    // TODO: Implement
}

void View::printGame() {
    // TODO: Implement
}