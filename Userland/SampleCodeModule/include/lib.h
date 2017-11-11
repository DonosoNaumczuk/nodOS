#ifndef _lib_userland_h_
#define _lib_userland_h_

#include <stdarg.h>
#include <stdint.h>
#include <syscall.h>

int printf(const char* c, ...);
int getchar();
unsigned char putChar(unsigned char c);
int scanf(const char* c, ...);
int	charToInt(char* string,int*	res);	//return 0 if string finish with 0, 1 if string finish with ' '
int pow(int base,unsigned int exponent);

#endif
