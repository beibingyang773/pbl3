#include <iostream>

#include "Game.h"

int main() {
    try {
        Game game(40, 20, "savegame.txt");
        game.Run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
