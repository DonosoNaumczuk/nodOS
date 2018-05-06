#ifndef _MEMORY_ALLOCATOR_H_

#define _MEMORY_ALLOCATOR_H_

#include <stdint.h>
#include <errors.h>
#include <bool.h>

#define MiB(x) (x * (1 << 20))
#define AVAILABLE_MEMORY_SIZE MiB(512) /* Must be power of two */
#define MIN_PAGE_SIZE 4096
#define PAGE_QUANTITY (AVAILABLE_MEMORY_SIZE / MIN_PAGE_SIZE)
#define FREE_MEMORY 0
#define USED_MEMORY 1
#define NULL (void *) 0
#define PARENT_INDEX(currentIndex) ((currentIndex + 1) / 2 - 1)
#define LEFT_CHILD_INDEX(currentIndex) (currentIndex * 2 + 1)
#define RIGHT_CHILD_INDEX(currentIndex) (currentIndex * 2 + 2)
#define IS_LEFT_CHILD(currentIndex) (currentIndex % 2 == 1)
#define IS_RIGHT_CHILD(currentIndex) (currentIndex % 2 == 0)
#define GET_SKIPPED_PAGES(addressToFree, memoryBaseAddress) (((uint64_t) addressToFree - \
										   (uint64_t)memoryBaseAddress) / \
										    MIN_PAGE_SIZE)

int initializeMemoryAllocator(void * baseAddress);
void * allocateMemory(uint64_t bytesToAllocate);
uint32_t freeMemory(void * addressToFree);

#endif
