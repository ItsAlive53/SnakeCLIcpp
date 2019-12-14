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
const char TILE_SNAKE_HEAD_LEFT = '<';
const char TILE_SNAKE_HEAD_UP = '^';
const char TILE_SNAKE_HEAD_RIGHT = '>';
const char TILE_SNAKE_HEAD_DOWN = 'V';
const char TILE_FRUIT = 'o';
const char BORDER_VERTICAL = '|';
const char BORDER_HORIZONTAL = '-';
const char BORDER_CORNER = '+';

// END Grid characters

// x is the column, y is the row. The origin (0,0) is top-left.
void setCursorPosition(int x, int y) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hOut, coord);
}

// How many milliseconds to wait before each frame
const int64_t MIN_MS_FRAMETIME = 1000 / 2;

int main() {
    // How many nanoseconds passed since last frame
    int64_t nsSinceLast = 0;

    // Could use bools, using bitmask for minimal memory savings
    char buttonMask = 0;

    // Has game over screen been drawn
    bool gameOverScreen = false;

    // Game instance
    SnakeGame game = SnakeGame(31, 15);

    // Main loop
    while (1) {
        // Reset button mask
        buttonMask = 0;

        // Get time at start of loop
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

#ifdef _WIN32
        // Check high bit of GetKeyState, if it's set, key is down, WASD/arrows = set correct bit of button mask
        if (GetKeyState('W') & 0x8000) buttonMask |= BUTTON_UP;
        if (GetKeyState('A') & 0x8000) buttonMask |= BUTTON_LEFT;
        if (GetKeyState('S') & 0x8000) buttonMask |= BUTTON_DOWN;
        if (GetKeyState('D') & 0x8000) buttonMask |= BUTTON_RIGHT;

        // Arrow keys
        if (GetKeyState(VK_UP) & 0x8000) buttonMask |= BUTTON_UP;
        if (GetKeyState(VK_LEFT) & 0x8000) buttonMask |= BUTTON_LEFT;
        if (GetKeyState(VK_DOWN) & 0x8000) buttonMask |= BUTTON_DOWN;
        if (GetKeyState(VK_RIGHT) & 0x8000) buttonMask |= BUTTON_RIGHT;

        // Restart game on R, if game has ended
        if (game.IsGameOver()) {
            if (GetKeyState('R') & 0x8000) {
                game.Reset();
                gameOverScreen = false;
            }
        }

        // Exit out of the loop on ESC
        if (GetKeyState(VK_ESCAPE) & 0x8000) break;
#else
        // TODO: Implement cross-platform input later
        // Would probably need a rewrite to run in a window, rather than a terminal
        // Non-blocking terminal input without waiting for delimiter, apparently fairly complicated

        // As a temporary fix, use completely random inputs
        if (nsSinceLast % 200 == 11) {
            game.ChangeDirection(SnakeGame::Direction::Right);
        } else if (nsSinceLast % 200 == 16) {
            game.ChangeDirection(SnakeGame::Direction::Up);
        } else if (nsSinceLast % 200 == 8) {
            game.ChangeDirection(SnakeGame::Direction::Down);
        } else if (nsSinceLast % 200 == 7) {
            game.ChangeDirection(SnakeGame::Direction::Left);
        }
#endif

        if (buttonMask & BUTTON_LEFT) game.ChangeDirection(SnakeGame::Direction::Left);
        if (buttonMask & BUTTON_UP) game.ChangeDirection(SnakeGame::Direction::Up);
        if (buttonMask & BUTTON_RIGHT) game.ChangeDirection(SnakeGame::Direction::Right);
        if (buttonMask & BUTTON_DOWN) game.ChangeDirection(SnakeGame::Direction::Down);

        // Break out of loop on exit button
        if (buttonMask & BUTTON_EXIT) break;

        if (nsSinceLast / 1000000 < MIN_MS_FRAMETIME) {
            // If timer for next frame hasn't been passed, do nothing
            nsSinceLast += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count();
            continue;
        }

        // Tick on every frame
        // (Unfortunately does tie tickrate to framerate, but since it's a command line game, it doesn't matter here)
        game.Tick();

        // If game ends, show last frame and quit drawing until restart
        if (game.IsGameOver() && gameOverScreen) {
            continue;
        }

#ifdef _WIN32
        // Use windows call to redraw over earlier screen to avoid unnecessary writes
        setCursorPosition(0, 0);
#elif
        // Clear screen, or at least portion of it
        printChar('\n', 8);

        // Warn user if running unsupported system
        std::cout << "Input is currently Windows-only, using random inputs\n";
#endif

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
                if (tile == SnakeGame::Tile::Snake) {
                    // Check if tile is snake head
                    SnakeGame::Position headPos = game.GetSnakeHeadPos();
                    if (i == (uint16_t)headPos.y && j == (uint16_t)headPos.x) {
                        // If tile is snake head, print directional head tile
                        SnakeGame::Direction snakeDir = game.GetSnakeDirection();
                        if (snakeDir == SnakeGame::Direction::Left) std::cout << TILE_SNAKE_HEAD_LEFT;
                        if (snakeDir == SnakeGame::Direction::Up) std::cout << TILE_SNAKE_HEAD_UP;
                        if (snakeDir == SnakeGame::Direction::Right) std::cout << TILE_SNAKE_HEAD_RIGHT;
                        if (snakeDir == SnakeGame::Direction::Down) std::cout << TILE_SNAKE_HEAD_DOWN;

                        // In case game hasn't started yet, print snake body tile
                        if (snakeDir == SnakeGame::Direction::None) std::cout << TILE_SNAKE;
                    } else {
                        // Not snake head, print snake body tile
                        std::cout << TILE_SNAKE;
                    }
                }
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

#ifdef _WIN32
        // If using windows-only drawing logic, make sure to clear fps counter trail
        printChar(' ', 16);
#endif

        // Set timer back by millisecond timer converted to nanos
        nsSinceLast -= MIN_MS_FRAMETIME * 1000000;
        nsSinceLast += span;

        // Basic game-over display
        if (game.IsGameOver()) {
            std::cout << "Game Over, press R to restart!\n";
            // If game ended, draw screen once and set bool
            gameOverScreen = true;
        }

        // Print newline and flush output stream at the end
        std::cout << std::endl;
    } // Main loop

    return 0;
}
