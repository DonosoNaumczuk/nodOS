#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void stringCopy(char *destination, char *source);
int stringLength(char *string);
char *cpuVendor(char *result);
char *stringConcatenation(const char *s1,const char *s2);
#endif
