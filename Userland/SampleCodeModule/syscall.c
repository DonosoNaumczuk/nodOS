#include <syscall.h>

void write(char * string, uint64_t length, char color){
	return _int_80(WRITE, string, length, color);
}

void read(int in_id, char * buffer, int qty_to_read) {
	return _int_80(READ, in_id, buffer, qty_to_read);
}

void printPixel(uint32_t x, uint32_t y, char color){
	return _int_80(WRITE_PIXEL, x, y, color);
}

uint64_t getResolutionX(){
	return _int_80(GET_RESOLUTION_X, 0, 0, 0);
}

uint64_t getResolutionY(){
	return _int_80(GET_RESOLUTION_Y, 0, 0, 0);
}

uint64_t getTime(int time){
	return _int_80(TIME, time,0,0);
}

void cleanScreen() {
	_int_80(CLEAR,0,0,0);
}

uint64_t createProcess(void * codeAddress, uint32_t parametersQuantity, void ** parametersVector) {
     return _int_80(CREATE_PROCESS, codeAddress, parametersQuantity, parametersVector);
}

int waitChild(uint64_t processId) {
     return _int_80(WAIT_CHILD_PROCESS, processId, 0, 0);
}

void * allocateMemory(uint64_t bytesToAllocate) {
     return _int_80(ALLOCATE_MEMORY, bytesToAllocate, 0, 0);
}

uint32_t freeMemory(void * addressToFree) {
     return _int_80(FREE, addressToFree, 0, 0);
}


int printAllProcess() {
     return _int_80(PRINT_ALL_PROCESS, 0, 0, 0);
}
