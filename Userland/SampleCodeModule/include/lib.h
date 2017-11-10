#ifndef _lib_userland_h_
#define _lib_userland_h_

#include <stdarg.h>
#include <stdint.h>
#include <syscall.h>

int printf(const char* c, ...);
int getchar();
char putChar(char c);
int scanf(const char* c, ...);

#endif
