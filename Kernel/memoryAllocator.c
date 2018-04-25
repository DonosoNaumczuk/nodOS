#include <memoryAllocator.h>

static void initializeHeapValues(memoryAllocator_t memoryAllocator)
static uint64_t getOffsetFromBaseAddress(memoryAllocator_t *memoryAllocator,
								  		 uint64_t bytesToAllocate);
static uint64_t calculateOffset(uint32_t index, uint32_t level,
	 							uint32_t heapLevels);
static void markUsedNodes(memoryAllocator_t *memoryAllocator,
						  uint32_t heapIndex);
static uint64_t nextPageSizeMultiple(uint64_t bytesToAllocate);
static uint32_t freeMemoryRecursive(uint8_t *heap,
									uint64_t skippedPages,
									uint64_t pageQuantityPerLevel,
									uint32_t heapIndex,
									uint32_t pagesSkipedAtBranch);
static void freeChildren(uint8_t *heap, uint32_t heapIndex);

memoryAllocator_t *initializeMemoryAllocator() {
	memoryAllocator_t memoryAllocator;
	memoryAllocator.heapLevels = HEAP_LEVELS;
	initializeHeapValues(memoryAllocator);
	return &memoryAllocator;
}

static void initializeHeapValues(memoryAllocator_t memoryAllocator) {
	for(int i = 0; i < HEAP_NODE_QUANTITY; i++) {
		memoryAllocator.heap[i] = FREE_MEMORY;
	}
}

void * allocateMemory(memoryAllocator_t *memoryAllocator,
	 				  uint64_t bytesToAllocate) {
    uint64_t offset;

	if(memoryAllocator == NULL || bytesToAllocate <= 0) {
		return NULL;
	}

	offset = getOffsetFromBaseAddress(memoryAllocator,
		 							  bytesToAllocate);

	if(error condition of offset) {
		return NULL;
	}

	return offset + BASE_ADDRESS;

}

static uint64_t getOffsetFromBaseAddress(memoryAllocator_t *memoryAllocator,
								  		 uint64_t bytesToAllocate) {
	uint32_t heapIndex = 0;
	uint32_t currentLevel = 0;
	uint64_t memoryRoundedUp = nextPageSizeMultiple(bytesToAllocate);
	uint64_t currentPageSize = AVAILABLE_MEMORY_SIZE;
	uint8_t memoryAllocated = FALSE;

	if(bytesToAllocate > AVAILABLE_MEMORY_SIZE) {
		return ERROR_STATE;
	}

	while(heapIndex >= 0 && !memoryAllocated) {
		if(memoryRoundedUp == currentPageSize) {
			if(memoryAllocator->heap[heapIndex] == FREE_MEMORY) {
				memoryAllocator->heap[heapIndex] = USED_MEMORY;
				markUsedNodes(memoryAllocator, heapIndex);
				memoryAllocated = TRUE;
			}
		}
		else if(memoryAllocator->heap[heapIndex] == FREE_MEMORY) {
				heapIndex = LEFT_CHILD_INDEX(heapIndex);
				currentPageSize /= 2;
				currentLevel++;
		}

		if(!memoryAllocated) {
			if(index % 2 == 0) {
				index++;
			}
			else {
				while(heapIndex % 2 == 1) {
					currentLevel--;
					currentPageSize *= 2;
					heapIndex = PARENT_INDEX(heapIndex);

					if(heapIndex < 0) {
						return ERROR_STATE;
					}
				}
			}
		}
	}

	return (memoryAllocated)? calculateOffset(currentIndex, currentLevel,
		 					  memoryAllocated->levels) : ERROR_STATE;
}

static uint64_t calculateOffset(uint32_t index, uint32_t level,
	 							uint32_t heapLevels) {
	uint64_t skippedPages = (currentIndex + 1 - (1 << level))
							 << (heapLevels - level);

	return skippedPages * MIN_PAGE_SIZE;
}

static void markUsedNodes(memoryAllocator_t *memoryAllocator,
						  uint32_t heapIndex) {
	uint32_t currentIndex = heapIndex;
	uint8_t freeMemoryFound = FALSE;

	do {
		currentIndex = (currentIndex % 2)? currentIndex + 1 : currentIndex - 1;

		if(currentIndex > 0 &&
		   (memoryAllocator->heap[currentIndex] == USED_MEMORY)) {
			   currentIndex = PARENT_INDEX(currentIndex);
			   memoryAllocator->heap[currentIndex] = USED_MEMORY;
		}
		else {
			freeMemoryFound = TRUE;
		}

	} while(!freeMemoryFound);
}

static uint64_t nextPageSizeMultiple(uint64_t bytesToAllocate) {
	uint64_t memoryRounded = MIN_PAGE_SIZE;

	if(bytesToAllocate > MIN_PAGE_SIZE) {
		while(memoryRounded <= AVAILABLE_MEMORY_SIZE &&
			  bytesToAllocate > memoryRounded) {
			memoryRounded *= 2;
		}
	}

	return memoryRounded;
}

uint32_t freeMemory(memoryAllocator_t *memoryAllocator, void * addressToFree) {
	uint64_t skippedPages = getSkippedPages(addressToFree);
	uint64_t pageQuantityPerLevel = PAGE_QUANTITY;

	if(skippedPages >= PAGE_QUANTITY) {
		return ERROR_STATE;
	}

	return freeMemoryRecursive(memoryAllocator->heap, skippedPages,
		 					   pageQuantityPerLevel, /* heapIndex: */ 0,
						   		/*pagesSkipedAtBranch*/ 0);
}

static uint32_t freeMemoryRecursive(uint8_t *heap,
									uint64_t skippedPages,
									uint64_t pageQuantityPerLevel,
									uint32_t heapIndex,
									uint32_t pagesSkipedAtBranch) {

	if(heap[heapIndex] == USED_MEMORY) {
		heap[heapIndex] = FREE_MEMORY;
		freeChildren(heap, heapIndex);
		return OK_STATE;
	}
	else {
		pageQuantityPerLevel /= 2;

		if(skippedPages < psgesSkippedAtBranch + pageQuantityPerLevel) {
			return freeMemoryRecursive(heap, skippedPages, pageQuantityPerLevel,
				 					   LEFT_CHILD_INDEX(heapIndex),
									   pagesSkipedAtBranch);

		}
		else {
			pagesSkipedAtBranch += pageQuantityPerLevel;
			return freeMemoryRecursive(heap, skippedPages, pageQuantityPerLevel,
									RIGHT_CHILD_INDEX(heapIndex),
									pagesSkipedAtBranch);
		}
	}
}

static void freeChildren(uint8_t *heap, uint32_t heapIndex) {
	heap[heapIndex] = FREE_MEMORY;
	if(heap[LEFT_CHILD_INDEX(heapIndex)] == FREE_MEMORY &&
	   heap[RIGHT_CHILD_INDEX(heapIndex)] == FREE_MEMORY) {
		return;
	}
	freeChildren(heap, LEFT_CHILD_INDEX(heapIndex));
	freeChildren(heap, RIGHT_CHILD_INDEX(heapIndex));
}
