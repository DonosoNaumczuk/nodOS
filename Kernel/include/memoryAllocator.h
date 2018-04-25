#ifndef _MEMORY_ALLOCATOR_H_

#define _MEMORY_ALLOCATOR_H_

#define AVAILABLE_MEMORY_SIZE MiB(512) //evans
#define MiB(x) (x * (1 << 20));
#define MIN_PAGE_SIZE 4096
#define PAGE_QUANTITY (AVAILABLE_MEMORY_SIZE / MIN_PAGE_SIZE)
#define HEAP_LEVELS (nextPowerOfTwo(AVAILABLE_MEMORY_SIZE) - \
					 nextPowerOfTwo(MIN_PAGE_SIZE))
#define HEAP_NODE_QUANTITY ((1 << levels) - 1)
#define FREE_MEMORY 0
#define USED_MEMORY 1
#define ERROR_STATE -1
#define OK_STATE 0
#define TRUE 1
#define FALSE 0
#define PARENT_INDEX(currentIndex) ((currentIndex + 1) / 2 - 1)
#define LEFT_CHILD_INDEX(currentIndex) (heapIndex * 2 + 1)

typedef struct {
	uint32_t heapLevels;
	uint8_t heap[HEAP_NODE_QUANTITY];
} memoryAllocator_t;

memoryAllocator_t *initializeMemoryAllocator();
void * allocateMemory(memoryAllocator_t *memoryAllocator,
					  uint64_t bytesToAllocate);
uint32_t freeMemory(memoryAllocator_t *memoryAllocator, void * addressToFree);

#endif
