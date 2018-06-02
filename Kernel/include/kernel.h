#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

void initialPrint();
void clearBSS(void * bssAddress, uint64_t bssSize);
void * getStackBase();
void * initializeKernelBinary();
void goToEntryPoint();
void init(int cant, void ** args);
void cleaner(int cant, void ** args);
int main();

#endif