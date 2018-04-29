#include "memoryAllocator.h"

static uint32_t getHeapLevels();
static uint8_t nextPowerOfTwo(uint64_t number);
static void initializeHeapValues(memoryAllocator_t memoryAllocator);
static uint64_t getOffsetFromBaseAddress(memoryAllocator_t *memoryAllocator,
								  		 uint64_t bytesToAllocate);
static uint64_t calculateOffset(int index, uint32_t level,
	 							uint32_t heapLevels);
static void markUsedNodes(memoryAllocator_t *memoryAllocator,
						  int heapIndex);
static uint64_t nextPageSizeMultiple(uint64_t bytesToAllocate);
static uint32_t freeMemoryRecursive(uint8_t *heap,
									uint64_t skippedPages,
									uint64_t pageQuantityPerLevel,
									uint32_t heapIndex,
									uint32_t pagesSkipedAtBranch);
static int calculateNextHeapIndexToSearch(int heapIndex, uint32_t* currentLevel,
												uint64_t *currentPageSize);
static uint32_t getBrotherIndex(uint32_t currentIndex);

static memoryAllocator_t memoryAllocator;

memoryAllocator_t *initializeMemoryAllocator(void * baseAddress) {
	memoryAllocator.heapLevels = getHeapLevels();
	memoryAllocator.heap = baseAddress;
	memoryAllocator.memoryBaseAddress = baseAddress + getHeapNodeQuantity();
	initializeHeapValues(memoryAllocator);
	return &memoryAllocator;
}

static void initializeHeapValues(memoryAllocator_t memoryAllocator) {
	for(uint64_t i = 0; i < getHeapNodeQuantity(); i++) {
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

	if(offset == ERROR_STATE) {
		return NULL;
	}

	return (void *)(offset + (uint64_t) memoryAllocator->memoryBaseAddress);

}

static uint64_t getOffsetFromBaseAddress(memoryAllocator_t *memoryAllocator,
								  		 uint64_t bytesToAllocate) {
	int heapIndex = 0;
	uint32_t currentLevel = 0;
	uint64_t memoryRoundedUp = nextPageSizeMultiple(bytesToAllocate);
	uint64_t currentPageSize = AVAILABLE_MEMORY_SIZE;
	uint8_t memoryAllocated = FALSE;
	if(bytesToAllocate > AVAILABLE_MEMORY_SIZE) {
		return ERROR_STATE;
	}
	while(heapIndex >= 0 && !memoryAllocated) {

		if(memoryAllocator->heap[heapIndex] == FREE_MEMORY) {


			if(memoryRoundedUp == currentPageSize) {
				memoryAllocator->heap[heapIndex] = USED_MEMORY;
				markUsedNodes(memoryAllocator, heapIndex);
				memoryAllocated = TRUE;
			}
			else {
				currentLevel ++;
				currentPageSize /= 2;
				if(memoryAllocator->heap[LEFT_CHILD_INDEX(heapIndex)] == FREE_MEMORY) {
					heapIndex = LEFT_CHILD_INDEX(heapIndex);
				}
				else {
					heapIndex = RIGHT_CHILD_INDEX(heapIndex);

				}
			}
		}
		else {
			heapIndex = calculateNextHeapIndexToSearch(heapIndex, &currentLevel, &currentPageSize);
			if(heapIndex < 0) {
				return ERROR_STATE;
			}
		}
	}

	return (memoryAllocated)? calculateOffset(heapIndex, currentLevel,
		 					  memoryAllocator->heapLevels) : ERROR_STATE;
}

static int calculateNextHeapIndexToSearch(int heapIndex, uint32_t* currentLevel,
												uint64_t *currentPageSize) {

	if(IS_LEFT_CHILD(heapIndex)) {
		heapIndex++;
	}
	else {
		while(IS_RIGHT_CHILD(heapIndex) && heapIndex >= 0) {
			(*currentLevel)--;
			(*currentPageSize) *= 2;
			heapIndex = PARENT_INDEX(heapIndex);
		}
	}
	return heapIndex;



}

static uint64_t calculateOffset(int heapIndex, uint32_t level,
	 							uint32_t heapLevels) {
	uint64_t skippedPages = (heapIndex + 1 - (1 << level))
							 << (heapLevels - level);

	return skippedPages * MIN_PAGE_SIZE;
}

static void markUsedNodes(memoryAllocator_t *memoryAllocator,
						  int heapIndex) {
	if(heapIndex > 0 && memoryAllocator->heap[getBrotherIndex(heapIndex)] == USED_MEMORY) {
		heapIndex = PARENT_INDEX(heapIndex);

		if(heapIndex >= 0) {
			memoryAllocator->heap[heapIndex] = USED_MEMORY;
		}
		markUsedNodes(memoryAllocator, heapIndex);
	}
}

static uint32_t getBrotherIndex(uint32_t currentIndex) {
	uint32_t brotherIndex;
	if(IS_LEFT_CHILD(currentIndex)) {
		brotherIndex = currentIndex + 1;
	}
	else {
		brotherIndex = currentIndex - 1;
	}
	return brotherIndex;
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
	uint64_t skippedPages = GET_SKIPPED_PAGES(addressToFree, memoryAllocator->memoryBaseAddress);
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
									uint32_t pagesSkippedAtBranch) {

	if(heap[heapIndex] == USED_MEMORY) {
		heap[heapIndex] = FREE_MEMORY;
		if(skippedPages == pagesSkippedAtBranch) {
			if(heap[LEFT_CHILD_INDEX(heapIndex)] == USED_MEMORY) {
				return freeMemoryRecursive(heap, skippedPages, pageQuantityPerLevel,
									LEFT_CHILD_INDEX(heapIndex),
									pagesSkippedAtBranch);
			}
		}
		else {
			if(heap[RIGHT_CHILD_INDEX(heapIndex)] == USED_MEMORY) {
				return freeMemoryRecursive(heap, skippedPages, pageQuantityPerLevel,
										   RIGHT_CHILD_INDEX(heapIndex),
										   pagesSkippedAtBranch);
			}
		}
		return OK_STATE;
	}
	else {
		pageQuantityPerLevel /= 2;

		if(skippedPages < pagesSkippedAtBranch + pageQuantityPerLevel) {
			return freeMemoryRecursive(heap, skippedPages, pageQuantityPerLevel,
				 					   LEFT_CHILD_INDEX(heapIndex),
									   pagesSkippedAtBranch);

		}
		else {
			pagesSkippedAtBranch += pageQuantityPerLevel;
			return freeMemoryRecursive(heap, skippedPages, pageQuantityPerLevel,
									RIGHT_CHILD_INDEX(heapIndex),
									pagesSkippedAtBranch);
		}
	}
}



static uint8_t nextPowerOfTwo(uint64_t number) {
	uint8_t power = 0;
	while(number - 1 > 0) {
		number /= 2;
		power ++;
	}
	return power;
}

static uint32_t getHeapLevels() {
	return (uint32_t) (nextPowerOfTwo(AVAILABLE_MEMORY_SIZE) -
		   			   nextPowerOfTwo(MIN_PAGE_SIZE));
}

uint64_t getHeapNodeQuantity() {
	return (1 << (getHeapLevels() + 1)) - 1;
}
