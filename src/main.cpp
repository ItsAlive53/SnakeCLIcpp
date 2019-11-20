#include <iostream> // std::cout
#include <chrono> // std::chrono::high_resolution_clock, std::chrono::duration_cast, std::chrono::nanoseconds
#include <vector> // mylib.h

#ifdef _WIN32
#include <Windows.h> // GetKeyState()
#endif

#include "mylib.h" // Helper functions
#include "game.h" // Game instance class


// Button bit positions

const char BUTTON_LEFT = 1;
const char BUTTON_UP = 1 << 1;
const char BUTTON_RIGHT = 1 << 2;
const char BUTTON_DOWN = 1 << 3;
const char BUTTON_EXIT = 1 << 7;

// END Button bit positions

// Grid characters

const char TILE_EMPTY = ' ';
const char TILE_SNAKE = 'S';
const char TILE_FRUIT = 'o';
const char BORDER_VERTICAL = '|';
const char BORDER_HORIZONTAL = '-';
const char BORDER_CORNER = '+';

// END Grid characters



// How many milliseconds to wait before each frame
const int64_t MIN_MS_FRAMETIME = 33;

int main() {
    // How many nanoseconds passed since last frame
    int64_t nsSinceLast = 0;

    // Could use bools, using bitmask for minimal memory savings
    char buttonMask = 0;

    // Game instance
    SnakeGame game = SnakeGame(63, 31);

    // Main loop
    while (1) {
        // Get time at start of loop
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        if (nsSinceLast / 1000000 < MIN_MS_FRAMETIME) {
            // If timer for next frame hasn't been passed, do nothing
            nsSinceLast += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count();
            continue;
        }

#ifdef _WIN32
        // Check high bit of GetKeyState, if it's set, key is down, WASD/arrows = set correct bit of button mask
        if (GetKeyState('W') & 0x8000) buttonMask |= BUTTON_UP;
        if (GetKeyState('A') & 0x8000) buttonMask |= BUTTON_LEFT;
        if (GetKeyState('S') & 0x8000) buttonMask |= BUTTON_DOWN;
        if (GetKeyState('D') & 0x8000) buttonMask |= BUTTON_RIGHT;
        if (GetKeyState(VK_UP) & 0x8000) buttonMask |= BUTTON_UP;
        if (GetKeyState(VK_LEFT) & 0x8000) buttonMask |= BUTTON_LEFT;
        if (GetKeyState(VK_DOWN) & 0x8000) buttonMask |= BUTTON_DOWN;
        if (GetKeyState(VK_RIGHT) & 0x8000) buttonMask |= BUTTON_RIGHT;

        // Exit out of the loop on ESC
        if (GetKeyState(VK_ESCAPE) & 0x8000) break;
#endif

#ifdef __linux__
        std::cout << "Input is not currently supported on linux-based systems\n";
#endif

        if (buttonMask & BUTTON_LEFT) game.ChangeDirection(SnakeGame::Direction::Left);
        if (buttonMask & BUTTON_UP) game.ChangeDirection(SnakeGame::Direction::Up);
        if (buttonMask & BUTTON_RIGHT) game.ChangeDirection(SnakeGame::Direction::Right);
        if (buttonMask & BUTTON_DOWN) game.ChangeDirection(SnakeGame::Direction::Down);

        // Break out of loop on exit button
        if (buttonMask & BUTTON_EXIT) break;

        // Tick on every frame
        // (Unfortunately does tie tickrate to framerate, but since it's a command line game, it doesn't matter here)
        game.Tick();

        // Clear screen, or at least portion of it
        printChar('\n', 32);

        // Basic game-over display
        if (game.IsGameOver()) std::cout << "Game Over! ";

        // Scoreboard
        std::cout << "Score: " << (int)game.GetScore() << '\n';

        // Print upper grid border
        printChar(BORDER_CORNER, 1);
        printChar(BORDER_HORIZONTAL, game.GetGridSizeHorizontal());
        printChar(BORDER_CORNER, 1);
        std::cout << '\n';

        // Print all rows
        for (uint16_t i = 0; i < game.GetGridSizeVertical(); i++) {
            // Leftmost grid border
            std::cout << BORDER_VERTICAL;

            // Print all columns in row i
            for (uint16_t j = 0; j < game.GetGridSizeHorizontal(); j++) {
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
        printChar(BORDER_HORIZONTAL, game.GetGridSizeHorizontal());
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
