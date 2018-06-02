#ifndef _lib_userland_h_
#define _lib_userland_h_

#include <stdarg.h>
#include <stdint.h>
#include <syscall.h>

int printf(const char* c, ...);
int getchar();
unsigned char putChar(char c);
int scanf(const char* c, ...);
int charToInt(unsigned char* string);
int pow(int base,unsigned int exponent);
void changeFontColor(int newColor);
int toBase(int n, char *pnt, int base);
int copyString(char* string, char* to);
int getNumber(int * dir, char * buffer, int * j);
int isNumber(char n);

#endif
