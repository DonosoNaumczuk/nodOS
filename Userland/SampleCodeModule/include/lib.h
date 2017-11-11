#ifndef _lib_userland_h_
#define _lib_userland_h_

#include <stdarg.h>
#include <stdint.h>
#include <syscall.h>

int printf(const char* c, ...);
int getchar();
unsigned char putChar(unsigned char c);
int scanf(const char* c, ...);
int	charToInt(unsigned char* string);
int pow(int base,unsigned int exponent);

#endif
