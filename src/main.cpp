#include <iostream> // std::cout

#include "game.h"

int main() {
    // Create new instance of game
    SnakeGame game = SnakeGame();

    // Main loop
    while (1) {
        game.Tick();
        break;
    } // Main loop
    std::cout << "WIP, check back later" << std::endl;

    return 0;
}
