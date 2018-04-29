#include "include/memoryAllocatorTest.h"

int main(int argc, char const *argv[]) {
	long memorySize = AVAILABLE_MEMORY_SIZE;
	void * memorySpace = malloc(memorySize + getHeapNodeQuantity());
	printf("BASE MEMORY ADDRESS - memorySpace = %p\n", memorySpace);
	memoryAllocator_t *memoryAllocator = initializeMemoryAllocator(memorySpace);

	testWithDiferentPageSize(memoryAllocator);
	fillMemoryWithPages4K(memoryAllocator, memorySize);
		
	return 0;
}

void testWithDiferentPageSize(memoryAllocator_t * memoryAllocator) {
	void * allocatedMemory1 = allocateMemory(memoryAllocator, 8192);
	if(allocatedMemory1 == NULL) {
		printf("Error memoria 1.\n");
	}
	else
	printf("FIRST ALLOCATION (100 Bytes) = %p\n", allocatedMemory1);

	void * allocatedMemory2 = allocateMemory(memoryAllocator, 200);
	if(allocatedMemory2 == NULL) {
		printf("Error memoria 2.\n");
	}
	else
	printf("SECOND ALLOCATION (200 Bytes)= %p\n", allocatedMemory2);



	void * allocatedMemory3 = allocateMemory(memoryAllocator, 400);
	printf("THIRD ALLOCATION (400 Bytes)= %p\n", allocatedMemory3);


	void * allocatedMemory4 = allocateMemory(memoryAllocator, 10000);
	printf("THIRD ALLOCATION (400 Bytes)= %p\n", allocatedMemory4);
	printf("\n\n");
	printMemory(memoryAllocator);
	
	freeMemory(memoryAllocator, allocatedMemory4);
	printf("\n\n");
	printMemory(memoryAllocator);
	freeMemory(memoryAllocator, allocatedMemory2);
	printf("\n\n");
	printMemory(memoryAllocator);

	freeMemory(memoryAllocator, allocatedMemory3);
	printf("\n\n");
	printMemory(memoryAllocator);
	freeMemory(memoryAllocator, allocatedMemory1);
	printf("\n\n");
	printMemory(memoryAllocator);
	
}

void printMemory(memoryAllocator_t * memoryAllocator) {
	printf("\n\n");
	for(int i = 0; i < getHeapNodeQuantity(); i++) {
		printf("memory[%d] = %d\n", i, memoryAllocator->heap[i]);
	}
	printf("\n\n");
}

void fillMemoryWithPages4K(memoryAllocator_t *memoryAllocator, long memorySize) {
	long pageQuantity = memorySize / 4096, i = 0;
	printf("Page quantity = %ld\n", pageQuantity);
	void * allocatedMemory[pageQuantity];
	for(i = 0; i < pageQuantity; i++) {
		allocatedMemory[i] = allocateMemory(memoryAllocator, 100);
		printf("%ld ALLOCATION (100 Bytes) = %p\n",i, allocatedMemory[i]);
		
	}
	printMemory(memoryAllocator);
	printf("\n\n");
	for(i = 0; i < pageQuantity; i++) {
		freeMemory(memoryAllocator, allocatedMemory[i]);
		printMemory(memoryAllocator);
		printf("\n\n");
	}

}
