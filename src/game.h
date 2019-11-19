#ifndef __GAME_INCLUDED__
#define __GAME_INCLUDED__

#include <vector>
#include <cstdint> // unit8_t, uint16_t

class SnakeGame {
public:
    struct Position {
        uint8_t x;
        uint8_t y;
    };

    enum class Direction: uint8_t { Up = 1, Down = 2, Left = 3, Right = 4, None = 0 };
    enum class Tile: uint8_t { Empty = 0, Snake = 1, Fruit = 2 };

    int MapGridSizeVertical;
    int MapGridSizeHorizontal;

    // Use default values ()
    SnakeGame();
    // Create perfect square (max 255)
    SnakeGame(int);
    // Create variable-size rectangle (max 255x255)
    SnakeGame(int, int);

    // Reset grid and create starting game state
    void Reset();
    // Run through game logic loop
    void Tick();
    // Returns current score
    uint16_t GetScore();
    // Sets current score
    void SetScore(uint16_t);
    // Adds parameter to current score (+/-)
    void ModifyScore(int);
    // Turn the snake (Does nothing if opposite current direction)
    void ChangeDirection(Direction);
    // Has the player died
    bool IsGameOver();
    // Returns grid width
    uint16_t GetGridSizeHorizontal();
    // Returns grid height
    uint16_t GetGridSizeVertical();
    // Returns tile at (x, y)
    Tile GetTile(int x, int y);

private:
    // Has the player died
    bool gameOver;
    // Map grid, for storing tile information
    std::vector<std::vector<uint8_t>> map;
    // Score counter
    uint16_t score;
    // Variable-size array for storing snake information
    std::vector<Position> snake;
    // How long the snake currently should be
    uint16_t snakeLength;
    // Where the snake is headed
    Direction snakeDirection;

    // Move snake by one tile
    void move();
    // Spawn new fruit randomly on grid
    void spawnFruit();

};

#endif // __GAME_INCLUDED__
