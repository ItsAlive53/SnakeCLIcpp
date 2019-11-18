#include <iostream> // std::cerr
#include <limits> // std::numeric_limits<T>
#include <vector> // std::vector<T>

#include "mylib.h"

#include "game.h"

const int MAP_GRID_SIZE = 31;

SnakeGame::SnakeGame() {
    Reset();
}

void SnakeGame::Reset() {
    /**
    *
    * Initialise game field map
    *
    * Map legend:
    *   0: Empty
    *   1: Spawn
    *
    * */
    for (uint8_t i = 0; i < (uint8_t)MAP_GRID_SIZE; i++) {
        for (uint8_t j = 0; j < (uint8_t)MAP_GRID_SIZE; j++) {
            this->map[i][j] = (uint8_t)SnakeGame::Tile::Empty;
        }
    }
    this->map[MAP_GRID_SIZE / 2][MAP_GRID_SIZE / 2] = (uint8_t)SnakeGame::Tile::Snake;

    // Reset basic vars
    this->score = 0;
    this->gameOver = false;

    // Reinitialise empty vector with snake part at the centre
    this->snake.clear();
    this->snake.resize(0);
    this->snake.push_back({MAP_GRID_SIZE / 2, MAP_GRID_SIZE / 2});

    // Set starting snake length
    this->snakeLength = 3;
    this->snakeDirection = SnakeGame::Direction::None;

    // Spawn first fruit
    this->spawnFruit();
}

void SnakeGame::Tick() {
    // Quit ticking if game over state reached
    if (this->gameOver) return;

    this->move();
}

uint16_t SnakeGame::GetScore() {
    return this->score;
}

void SnakeGame::SetScore(uint16_t newScore) {
    this->score = newScore;
}

void SnakeGame::ModifyScore(int amount) {
    // Use temporary variable to verify score won't over- or underflow
    int temp = static_cast<int>(this->score);
    temp += amount;

    // Clamp temp variable to [0, 2^8)
    if (temp < 0) {
        temp = 0;
    } else if (temp > std::numeric_limits<uint8_t>::max()) {
        temp = std::numeric_limits<uint8_t>::max();
    }

    // Set sanitised temporary variable as the new score value
    this->score = static_cast<uint16_t>(temp);
}

void SnakeGame::ChangeDirection(Direction newDir) {
    // If new direciton would be opposite current direction, do nothing
    if (newDir == SnakeGame::Direction::Left && this->snakeDirection == SnakeGame::Direction::Right) return;
    if (newDir == SnakeGame::Direction::Right && this->snakeDirection == SnakeGame::Direction::Left) return;
    if (newDir == SnakeGame::Direction::Up && this->snakeDirection == SnakeGame::Direction::Down) return;
    if (newDir == SnakeGame::Direction::Down && this->snakeDirection == SnakeGame::Direction::Up) return;

    this->snakeDirection = newDir;
}

bool SnakeGame::IsGameOver() {
    return this->gameOver;
}

void SnakeGame::move() {
    if (this->snakeDirection == SnakeGame::Direction::None) return;

    // Simple safety check
    if (this->snake.size() <= 0) {
        std::cerr << "Something went wrong with snake vector" << std::endl;
        return;
    }

    SnakeGame::Position newPos = this->snake.back();

    // Directional movement
    if (this->snakeDirection == SnakeGame::Direction::Left) {
        // Loop through walls on hit
        if (newPos.x == 0) newPos.x = MAP_GRID_SIZE;
        else newPos.x--;
    }
    if (this->snakeDirection == SnakeGame::Direction::Up) {
        // Loop through walls on hit
        if (newPos.y == MAP_GRID_SIZE) newPos.y = 0;
        else newPos.y++;
    }
    if (this->snakeDirection == SnakeGame::Direction::Right) {
        // Loop through walls on hit
        if (newPos.x == MAP_GRID_SIZE) newPos.x = 0;
        else newPos.x++;
    }
    if (this->snakeDirection == SnakeGame::Direction::Down) {
        // Loop through walls on hit
        if (newPos.y == 0) newPos.y = MAP_GRID_SIZE;
        else newPos.y--;
    }
    // END Directional movement

    if (this->map[newPos.y][newPos.x] == (uint8_t)SnakeGame::Tile::Fruit) {
        // Increment score by 1 and spawn new fruit
        this->ModifyScore(1);
        this->spawnFruit();
    } else if (this->map[newPos.y][newPos.x] == (uint8_t)SnakeGame::Tile::Snake) {
        // Snake hit itself, end game
        this->gameOver = true;
    }

    // Remove tail bit when snake moves, if max size was reached
    if (this->snake.size() > this->snakeLength) {
        this->map[this->snake[0].y][this->snake[0].x] = (uint8_t)SnakeGame::Tile::Empty;
        this->snake.erase(this->snake.begin());
    }
}

void SnakeGame::spawnFruit() {
    // How many random numbers to get every iteration
    int spawnedPotentials = 8;

    bool found = false;

    int coordX = 0;
    int coordY = 0;

    while (!found) {
        std::vector<int> potentialCoordsX = getUniqueRandomNumbers(spawnedPotentials, 0, 31);
        std::vector<int> potentialCoordsY = getUniqueRandomNumbers(spawnedPotentials, 0, 31);

        int i = 0;

        for (int i = 0; i < spawnedPotentials; i++) {
            coordX = potentialCoordsX[i];
            coordY = potentialCoordsY[i];

            if (map[coordY][coordX] == (uint8_t)SnakeGame::Tile::Empty) {
                found = true;
                break;
            }
        }
    }

    map[coordY][coordX] = (uint8_t)SnakeGame::Tile::Fruit;
}
