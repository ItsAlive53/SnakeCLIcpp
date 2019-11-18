#include <iostream> // std::cout
#include <vector> // mylib.h
#include <chrono>

#include "mylib.h"
#include "game.h"

// Define grid characters
const char TILE_EMPTY = ' ';
const char TILE_SNAKE = 'S';
const char TILE_FRUIT = 'o';
const char BORDER_VERTICAL = '|';
const char BORDER_HORIZONTAL = '-';
const char BORDER_CORNER = '+';

// How many milliseconds to wait before each frame
const int64_t MIN_MS_FRAMETIME = 33;

int main() {
    int64_t nsSinceLast = 0;

    // Create new instance of game
    SnakeGame game = SnakeGame();

    // Main loop
    while (1) {
        // Get time at start of loop
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        if (nsSinceLast / 1000000 < MIN_MS_FRAMETIME) {
            // If timer for next frame hasn't been passed, do nothing
            nsSinceLast += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count();
            continue;
        }

        // For now, tick on every frame
        game.Tick();

        // Print upper grid border
        printChar(BORDER_CORNER, 1);
        printChar(BORDER_HORIZONTAL, game.GetGridSize());
        printChar(BORDER_CORNER, 1);
        std::cout << '\n';

        // Print all rows
        for (uint16_t i = 0; i < game.GetGridSize(); i++) {
            // Leftmost grid border
            std::cout << BORDER_VERTICAL;
            for (uint16_t j = 0; j < game.GetGridSize(); j++) {
                // Print columns according to tile
                SnakeGame::Tile tile = game.GetTile(j, i);
                if (tile == SnakeGame::Tile::Empty) std::cout << TILE_EMPTY;
                if (tile == SnakeGame::Tile::Snake) std::cout << TILE_SNAKE;
                if (tile == SnakeGame::Tile::Fruit) std::cout << TILE_FRUIT;
            }
            // Rightmost grid border
            std::cout << BORDER_VERTICAL << '\n';
        }
        // Print bottom grid border
        printChar(BORDER_CORNER, 1);
        printChar(BORDER_HORIZONTAL, game.GetGridSize());
        printChar(BORDER_CORNER, 1);

        // Get timespan between start of loop, and here
        int64_t span = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count();

        // Print frametime for reference
        std::cout << '\n'
        << (float)span / 1000000
        << " ms ("
        << (float)nsSinceLast / 1000000
        << " total ms) ("
        << 1.f / ((float) nsSinceLast / 1000000000)
        << " fps)";

        // Set timer back by millisecond timer converted to nanos
        nsSinceLast -= MIN_MS_FRAMETIME * 1000000;
        nsSinceLast += span;

        // Print newline and flush output stream at the end
        std::cout << std::endl;
    } // Main loop

    return 0;
}
