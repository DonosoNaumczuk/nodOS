#ifndef SCREEN_DRIVER_H
#define SCREEN_DRIVER_H

#include <stdint.h>

void printCharWithColor(char character, char font, char background);
void moveUP();
void printWithColor(char * string, char font, char background);
void newLine();
void clear();
void printRainbow(char * string);

#endif