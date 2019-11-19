#ifndef __MYLIB_INCLUDED__
#define __MYLIB_INCLUDED__

// Fetch a string from std::istream
std::string getString(std::istream& istream, std::string promptMessage);

// Return n random numbers from range [min, max], either unique or any
std::vector<int> getRandomNumbers(int amount, int min, int max, bool unique = false);
// Return n random unique numbers from range [min, max]
// Alias for getRandomNumbers(amount, min, max, true)
std::vector<int> getUniqueRandomNumbers(int amount, int min, int max);

// Sort an int vector smallest to largest
std::vector<int> sortVectorInt(std::vector<int> vec);

// Get average from array of integers
int calcAvgInt(int *arr, size_t arrSize);
// Get largest number in int array
int getMaxInt(int *arr, size_t arrSize);
// Get largest number in 16-bit unsigned int array
uint16_t getMaxUInt16(uint16_t *arr, size_t arrSize);
// Get smallest number in int array
int getMinInt(int *arr, size_t arrSize);
// Get smallest number in 16-bit unsigned int array
uint16_t getMinUInt16(uint16_t *arr, size_t arrSize);

// Print n amount of character c, without newline
// Alias for printChar(c, amount, false)
void printChar(char c, uint16_t amount);
// Print n amount of character c, with or without trailing newline
void printChar(char c, uint16_t amount, bool newLine);

#endif // #ifndef __MYLIB_INCLUDED__
