#include <iostream> // std::cerr
#include <limits> // std::numeric_limits<T>
#include <vector> // std::vector<T>

#include "mylib.h" // Helper functions

#include "game.h" // Class declaration

// Default grid of 31x31
SnakeGame::SnakeGame() : SnakeGame(31, 31) {}

// Square grid
// Max 255
SnakeGame::SnakeGame(int size) : SnakeGame(size, size) {}

// Rectangle grid
// Max 255, 255
SnakeGame::SnakeGame(int x, int y) {
    // Set grid sizes
    this->MapGridSizeHorizontal = x;
    this->MapGridSizeVertical = y;

    // Create starting grid
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
    // Resize vertical vector
    map.resize(this->MapGridSizeVertical);
    for (uint8_t i = 0; i < (uint8_t)MapGridSizeVertical; i++) {
        // Resize horizontal vectors
        map[i].resize(this->MapGridSizeHorizontal);
        for (uint8_t j = 0; j < (uint8_t)MapGridSizeHorizontal; j++) {
            // Blank every tile at beginning
            this->map[i][j] = (uint8_t)SnakeGame::Tile::Empty;
        }
    }

    // Create snake head at centre tile
    this->map[MapGridSizeVertical / 2][MapGridSizeHorizontal / 2] = (uint8_t)SnakeGame::Tile::Snake;

    // Reset basic vars
    this->score = 0;
    this->gameOver = false;

    // Reinitialise empty vector with snake part at the centre
    this->snake.clear();
    this->snake.resize(0);
    this->snake.push_back({MapGridSizeVertical / 2, MapGridSizeHorizontal / 2});

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

uint16_t SnakeGame::GetGridSizeVertical() {
    return (uint16_t)this->MapGridSizeVertical;
}

uint16_t SnakeGame::GetGridSizeHorizontal() {
    return (uint16_t)this->MapGridSizeHorizontal;
}

SnakeGame::Tile SnakeGame::GetTile(int x, int y) {
    return (SnakeGame::Tile)this->map[y][x];
}

void SnakeGame::move() {
    if (this->snakeDirection == SnakeGame::Direction::None) return;

    // Simple safety check
    if (this->snake.size() <= 0) {
        std::cerr << "Something went wrong with snake vector" << std::endl;
        return;
    }

    SnakeGame::Position newPos = {this->snake.back().x, this->snake.back().y};

    // Directional movement
    if (this->snakeDirection == SnakeGame::Direction::Left) {
        // Loop through walls on hit
        if (newPos.x == 0) newPos.x = MapGridSizeHorizontal;
        else newPos.x--;
    }
    if (this->snakeDirection == SnakeGame::Direction::Up) {
        // Loop through walls on hit
        if (newPos.y >= MapGridSizeVertical - 1) newPos.y = 0;
        else newPos.y++;
    }
    if (this->snakeDirection == SnakeGame::Direction::Right) {
        // Loop through walls on hit
        if (newPos.x >= MapGridSizeHorizontal - 1) newPos.x = 0;
        else newPos.x++;
    }
    if (this->snakeDirection == SnakeGame::Direction::Down) {
        // Loop through walls on hit
        if (newPos.y == 0) newPos.y = MapGridSizeVertical;
        else newPos.y--;
    }
    // END Directional movement

    if (this->map[newPos.y][newPos.x] == (uint8_t)SnakeGame::Tile::Fruit) {
        // Increment score by 1 and spawn new fruit
        this->ModifyScore(1);
        this->spawnFruit();

        // Grow snake
        this->snakeLength++;

        // Move snake after score increment and new fruit spawn
        this->map[newPos.y][newPos.x] = (uint8_t)SnakeGame::Tile::Snake;
        this->snake.push_back({newPos.x, newPos.y});
    } else if (this->map[newPos.y][newPos.x] == (uint8_t)SnakeGame::Tile::Snake) {
        // Snake hit itself, end game
        this->gameOver = true;
    } else if (this->map[newPos.y][newPos.x] == (uint8_t)SnakeGame::Tile::Empty) {
        // Move snake normally
        this->map[newPos.y][newPos.x] = (uint8_t)SnakeGame::Tile::Snake;
        this->snake.push_back({newPos.x, newPos.y});
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

    // Has a suitable location been found?
    bool found = false;

    // Temporary variables used to find where to spawn fruit
    int coordX = 0;
    int coordY = 0;

    // Variables used to generate random numbers to iterate over
    std::vector<int> potentialCoordsX;
    std::vector<int> potentialCoordsY;

    // Loop until suitable location found
    while (!found) {
        potentialCoordsX = getUniqueRandomNumbers(spawnedPotentials, 0, MapGridSizeHorizontal - 1);
        potentialCoordsY = getUniqueRandomNumbers(spawnedPotentials, 0, MapGridSizeVertical - 1);

        for (int i = 0; i < spawnedPotentials; i++) {
            coordX = potentialCoordsX[i];
            coordY = potentialCoordsY[i];

            if (map[coordY][coordX] == (uint8_t)SnakeGame::Tile::Empty) {
                // Suitable spawn point found, return to program
                found = true;
                break;
            }
        }

        // Empty arrays in case function needs to be run again
        potentialCoordsX.clear();
        potentialCoordsX.resize(0);
        potentialCoordsY.clear();
        potentialCoordsY.resize(0);
    }

    // Set found tile to fruit
    map[coordY][coordX] = (uint8_t)SnakeGame::Tile::Fruit;
}
