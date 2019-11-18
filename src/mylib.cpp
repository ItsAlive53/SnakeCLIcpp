#include <string>
#include <iostream>
#include <algorithm> // std::find
#include <random>
#include <chrono>
#include <vector>

#include "mylib.h"

std::string getString(std::istream& istream, std::string promptMessage) {
    std::string input;

    std::cout << promptMessage;
    std::getline(istream, input);

    return input;
}

char getChar(std::istream& istream, std::string promptMessage) {
    std::string input;

    std::cout << promptMessage;
    std::getline(istream, input);

    return input[0];
}

int getInt(std::istream& istream, std::string promptMessage) {
    std::string input;
    int ret;
    bool invalid = false;

    while (1) {
        std::cout << promptMessage;
        std::getline(istream, input);

        try {
            ret = std::stoi(input, nullptr, 10);
        } catch (std::invalid_argument) {
            std::cout << "Invalid input, try again" << std::endl;
            invalid = true;
        }

        if (invalid) invalid = false;
        else return ret;
    }
}

uint16_t getUInt16(std::istream& istream, std::string promptMessage) {
    while (1) {
        int x = getInt(istream, promptMessage);

        if (x < 0) std::cout << "Input cannot be negative, try again" << std::endl;
        else if (x > 65535) std::cout << "Input too large, try again" << std::endl;
        else return static_cast<uint16_t>(x);
    }
}

float getFloat(std::istream& istream, std::string promptMessage) {
    std::string input;
    float ret;
    bool invalid = false;

    while (1) {
        std::cout << promptMessage;
        std::getline(istream, input);

        try {
            ret = std::stof(input, nullptr);
        } catch (std::invalid_argument) {
            std::cout << "Invalid input, try again" << std::endl;
            invalid = true;
        }

        if (invalid) invalid = false;
        else return ret;
    }
}

double getDouble(std::istream& istream, std::string promptMessage) {
    std::string input;
    double ret;
    bool invalid = false;

    while (1) {
        std::cout << promptMessage;
        std::getline(istream, input);

        try {
            ret = std::stod(input, nullptr);
        } catch (std::invalid_argument) {
            std::cout << "Invalid input, try again" << std::endl;
            invalid = true;
        }

        if (invalid) invalid = false;
        else return ret;
    }
}

std::vector<int> getRandomNumbers(int amount, int min, int max, bool unique) {
    std::vector<int> ret;

    uint32_t rdgen = 0;
    try {
        std::random_device rd;
        rdgen = rd();
    } catch (std::exception) {
        // implementation or device doesn't support random_device, fall back to current timestamp
        std::chrono::high_resolution_clock::time_point curTime = std::chrono::high_resolution_clock::now();
        rdgen = static_cast<uint32_t>(std::chrono::time_point_cast<std::chrono::microseconds>(curTime).time_since_epoch().count());
    }

    std::chrono::high_resolution_clock::time_point curTime = std::chrono::high_resolution_clock::now();
    uint32_t time = static_cast<uint32_t>(std::chrono::time_point_cast<std::chrono::nanoseconds>(curTime).time_since_epoch().count());

    uint64_t rngSeed = (uint64_t)rdgen << 32 | time; // add time-based entropy in case random_device implementation fails to give a true random number

    std::mt19937_64 prng(rngSeed);
    std::uniform_int_distribution<int> dist(min, max); // limits minimum-inclusive, maximum-inclusive, [min, max]

    for (int i = 0; i < amount; i++) {
        int gen = dist(prng);

        if (unique && std::find(ret.begin(), ret.end(), gen) != ret.end()) { // if (value is in vector)
            i--;
        } else {
            ret.push_back(gen);
        }
    }

    return ret;
}

std::vector<int> getUniqueRandomNumbers(int amount, int min, int max) {
    return getRandomNumbers(amount, min, max, true);
}

std::vector<int> sortVectorInt(std::vector<int> vec) {
    // very basic and inefficient sort function

    std::vector<int> sorted;
    int sz = (int)vec.size();

    for (int i = 0; i < sz; i++) {
        int min = vec[0];
        int pos = 0;

        for (int ii = 0; ii < vec.size(); ii++) {
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

bool isPrimeNumberInt(int num) {
    if (num < 0) num *= -1; // calculate with positive values

    // get the basic calculations out of the way quickly
    if (num <= 3 && num != 0) return true;
    if (num == 5) return true;
    if (num % 2 == 0 || num % 3 == 0 || num % 5 == 0) return false;

    // if number doesnt fit the easy definitions, start looping
    for (int i = 7; i*i <= num; i+=2) {
        if (num % i == 0) return false;
    }

    // if loop never returns, it's a prime
    return true;
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
    printChar(c, amount, false);
}

void printChar(char c, uint16_t amount, bool newLine) {
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
