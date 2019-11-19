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

    // Use default values
    SnakeGame();
    // Create perfect square
    SnakeGame(int);
    // Create variable-size rectangle
    SnakeGame(int, int);

    void Reset();
    void Tick();
    uint16_t GetScore();
    void SetScore(uint16_t);
    void ModifyScore(int);
    void ChangeDirection(Direction);
    bool IsGameOver();
    uint16_t GetGridSizeHorizontal();
    uint16_t GetGridSizeVertical();
    Tile GetTile(int x, int y);

private:
    bool gameOver;
    std::vector<std::vector<uint8_t>> map;
    uint16_t score;
    std::vector<Position> snake;
    uint16_t snakeLength;
    Direction snakeDirection;

    void move();
    void spawnFruit();

};

#endif // __GAME_INCLUDED__
