#ifndef __MYLIB_INCLUDED__
#define __MYLIB_INCLUDED__

std::string getString(std::istream& istream, std::string promptMessage);
char getChar(std::istream& istream, std::string promptMessage);
int getInt(std::istream& istream, std::string promptMessage);
uint16_t getUInt16(std::istream& istream, std::string promptMessage);
float getFloat(std::istream& istream, std::string promptMessage);
double getDouble(std::istream& istream, std::string promptMessage);

std::vector<int> getRandomNumbers(int amount, int min, int max, bool unique = false);
std::vector<int> getUniqueRandomNumbers(int amount, int min, int max);

std::vector<int> sortVectorInt(std::vector<int> vec);

bool isPrimeNumberInt(int num);

int calcAvgInt(int *arr, size_t arrSize);
int getMaxInt(int *arr, size_t arrSize);
uint16_t getMaxUInt16(uint16_t *arr, size_t arrSize);
int getMinInt(int *arr, size_t arrSize);
uint16_t getMinUInt16(uint16_t *arr, size_t arrSize);

void printChar(char c, uint16_t amount);
void printChar(char c, uint16_t amount, bool newLine);

#endif // #ifndef __MYLIB_INCLUDED__
