#include <stdint.h>
#include <int80.h>

void write(char * string, uint64_t length, char color);
//read
void printPixel(uint32_t x, uint32_t y, char color);
uint64_t getResolutionX();
uint64_t getResolutionY();
uint64_t getTime(int time);
void* malloc(int size);
void free();
void cleanScreen();