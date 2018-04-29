#ifndef _MEMORY_ALLOCATOR_H_

#define _MEMORY_ALLOCATOR_H_

#include <stdint.h>

#define MiB(x) (x * (1 << 20))
#define AVAILABLE_MEMORY_SIZE 32768//MiB(512)
#define MIN_PAGE_SIZE 4096
#define PAGE_QUANTITY (AVAILABLE_MEMORY_SIZE / MIN_PAGE_SIZE)
#define FREE_MEMORY 0
#define USED_MEMORY 1
#define ERROR_STATE -1
#define OK_STATE 0
#define NULL (void *) 0
#define TRUE 1
#define FALSE 0
#define PARENT_INDEX(currentIndex) ((currentIndex + 1) / 2 - 1)
#define LEFT_CHILD_INDEX(currentIndex) (currentIndex * 2 + 1)
#define RIGHT_CHILD_INDEX(currentIndex) (currentIndex * 2 + 2)
#define IS_LEFT_CHILD(currentIndex) (currentIndex % 2 == 1)
#define IS_RIGHT_CHILD(currentIndex) (currentIndex % 2 == 0)
#define GET_SKIPPED_PAGES(addressToFree, memoryBaseAddress) (((uint64_t) addressToFree - \
										   (uint64_t)memoryBaseAddress) / \
										    MIN_PAGE_SIZE)

typedef struct {
    void * memoryBaseAddress;
	uint32_t heapLevels;
	uint8_t *heap;
} memoryAllocator_t;

memoryAllocator_t *initializeMemoryAllocator(void * baseAddress);
void * allocateMemory(memoryAllocator_t *memoryAllocator,
					  uint64_t bytesToAllocate);
uint32_t freeMemory(memoryAllocator_t *memoryAllocator, void * addressToFree);
uint64_t getHeapNodeQuantity();

#endif
