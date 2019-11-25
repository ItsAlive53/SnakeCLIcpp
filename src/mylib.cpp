#include <string> // std::string
#include <iostream> // std::cout, std::istream, std::getline
#include <algorithm> // std::find
#include <random> // std::mt19937_64, std::uniform_int_distribution, std::random_device
#include <chrono> // std::chrono::high_resolution_clock, std::chrono::time_point_cast, std::chrono::(nano/micro)seconds
#include <vector> // std::vector<T>

#include "mylib.h" // Function declarations

std::string getString(std::istream& istream, std::string promptMessage) {
    std::string input;

    std::cout << promptMessage;
    std::getline(istream, input);

    return input;
}

std::vector<int> getRandomNumbers(int amount, int min, int max, bool unique) {
    // Return vector
    std::vector<int> ret;

    // Verify values are valid
    if (max < min || (unique && max - min < amount)) {
        throw new std::invalid_argument("Invalid arguments, or too small distribution for requested amount of unique numbers");

        // Return empty vector just in case
        return ret;
    }

    // PRNG seed
    uint32_t rdgen = 0;
    try {
        // Get true random number, if supported, for the seed
        std::random_device rd;
        rdgen = rd();
    } catch (std::exception) {
        // Implementation or device doesn't support random_device, fall back to current timestamp
        std::chrono::high_resolution_clock::time_point curTime = std::chrono::high_resolution_clock::now();
        rdgen = static_cast<uint32_t>(std::chrono::time_point_cast<std::chrono::microseconds>(curTime).time_since_epoch().count());
    }

    // Get current timestamp as second part of seed
    std::chrono::high_resolution_clock::time_point curTime = std::chrono::high_resolution_clock::now();
    uint32_t time = static_cast<uint32_t>(std::chrono::time_point_cast<std::chrono::nanoseconds>(curTime).time_since_epoch().count());

    // OR bit-shifted 32-bit timestamp onto seed, in case random_device fails
    uint64_t rngSeed = (uint64_t)rdgen << 32 | time;

    // Create PRNG engine with seed
    std::mt19937_64 prng(rngSeed);

    // Create uniform distribution from PRNG, limits inclusive [min, max]
    std::uniform_int_distribution<int> dist(min, max); 

    // Run through generation i times, defined by amount parameter
    for (int i = 0; i < amount; i++) {
        // Generate number
        int gen = dist(prng);

        // If unique is set, only add unique random numbers
        if (unique && std::find(ret.begin(), ret.end(), gen) != ret.end()) { // if (value is in vector)
            i--;
        } else {
            ret.push_back(gen);
        }
    }

    return ret;
}

std::vector<int> getUniqueRandomNumbers(int amount, int min, int max) {
    // Run random number gen with unique set
    return getRandomNumbers(amount, min, max, true);
}

std::vector<int> sortVectorInt(std::vector<int> vec) {
    // Very basic and inefficient sort function

    std::vector<int> sorted;
    int sz = (int)vec.size();

    for (int i = 0; i < sz; i++) {
        int min = vec[0];
        int pos = 0;

        for (int ii = 0; ii < sz; ii++) {
            if (vec[ii] < min) {
                min = vec[ii];
                pos = ii;
            }
        }

        sorted.push_back(min);
        vec.erase(vec.begin() + pos);
    }

    return sorted;
}

int calcAvgInt(int *arr, size_t arrSize) {
    if (arrSize <= 0) return 0;

    int total = 0;

    for (int i = 0; i < (int)arrSize; i++) {
        total += arr[i];
    }

    return total / (int)arrSize;
}

int getMaxInt(int *arr, size_t arrSize) {
    if (arrSize <= 0) return 0;

    int max = arr[0];
    for (int i = 1; i < (int)arrSize; i++) {
        max = arr[i] > max ? arr[i] : max;
    }

    return max;
}

uint16_t getMaxUInt16(uint16_t *arr, size_t arrSize) {
    if (arrSize <= 0) return 0;

    uint16_t max = arr[0];
    for (uint16_t i = 1; i < (uint16_t)arrSize; i++) {
        max = arr[i] > max ? arr[i] : max;
    }

    return max;
}

int getMinInt(int *arr, size_t arrSize) {
    if (arrSize <= 0) return 0;

    int min = arr[0];
    for (int i = 1; i < (int)arrSize; i++) {
        min = arr[i] < min ? arr[i] : min;
    }

    return min;
}

uint16_t getMinUInt16(uint16_t *arr, size_t arrSize) {
    if (arrSize <= 0) return 0;

    uint16_t min = arr[0];
    for (uint16_t i = 1; i < (uint16_t)arrSize; i++) {
        min = arr[i] < min ? arr[i] : min;
    }

    return min;
}

void printChar(char c, uint16_t amount) {
    // Run function without newLine set
    printChar(c, amount, false);
}

void printChar(char c, uint16_t amount, bool newLine) {
    // Don't print nulls
    if (c == '\0') {
        return;
    }

    for (uint16_t i = 0; i < amount; i++) {
        std::cout << c;
    }

    if (newLine) {
        std::cout << std::endl;
    }
}
