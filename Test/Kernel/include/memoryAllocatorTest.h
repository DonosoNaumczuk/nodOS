#ifndef MEMORY_ALLOCATOR_TEST_H
#define MEMORY_ALLOCATOR_TEST_H

#include "memoryAllocator.h"
#include <stdlib.h>
#include <stdio.h>

void fillMemoryWithPages4K(memoryAllocator_t *memoryAllocator, long memorySize);
void printMemory(memoryAllocator_t * memoryAllocator);
void testWithDiferentPageSize(memoryAllocator_t * memoryAllocator);

#endif

