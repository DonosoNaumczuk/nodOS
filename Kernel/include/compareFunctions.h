#ifndef COMPARE_FUNCTIONS_H
#define COMPARE_FUNCTIONS_H

#include <stdint.h>

#define EQUALS 0
#define NULL 0

int stringCompare(char *string, char *otherString);
int uintCompare(uint64_t number, uint64_t otherNumber);

#endif
