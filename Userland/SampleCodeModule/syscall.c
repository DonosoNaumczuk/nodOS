#include <syscall.h>

void write(char * string, uint64_t length, char color){
	_int_80(WRITE, (uint64_t) string, length, color);
}

void read(int in_id, char * buffer, int qty_to_read) {
	_int_80(READ, in_id, (uint64_t) buffer, qty_to_read);
}

void printPixel(uint32_t x, uint32_t y, char color){
	_int_80(WRITE_PIXEL, x, y, color);
}

uint64_t getResolutionX(){
	return _int_80(GET_RESOLUTION_X, 0, 0, 0);
}

uint64_t getResolutionY(){
	return _int_80(GET_RESOLUTION_Y, 0, 0, 0);
}

uint64_t getTime(int time){
	return _int_80(TIME_SYSCALL, time,0,0);
}

void cleanScreen() {
	_int_80(CLEAR,0,0,0);
}

uint64_t terminateCurrentProcess() {
     return _int_80(TERMINATE_CURRENT_PROCESS, 0, 0, 0);
}

uint64_t createProcess(void * codeAddress, uint32_t parametersQuantity, void ** parametersVector) {
     return _int_80(CREATE_PROCESS, (uint64_t) codeAddress, parametersQuantity,
                     (uint64_t) parametersVector);
}

int sleepProcess() {
     return _int_80(SLEEP_PROCESS, 0, 0, 0);
}

int wakeUpProcess(uint64_t processId) {
     return _int_80(WAKE_UP_PROCESS, processId, 0, 0);
}

int waitChild(uint64_t processId) {
     return _int_80(WAIT_CHILD_PROCESS, processId, 0, 0);
}

int getPid() {
     return _int_80(GET_PID, 0, 0, 0);
}

void * allocateMemory(uint64_t bytesToAllocate) {
     return (void * ) _int_80(ALLOCATE_MEMORY, bytesToAllocate, 0, 0);
}

uint32_t freeMemory(void * addressToFree) {
     return _int_80(FREE, (uint64_t) addressToFree, 0, 0);
}

int printAllProcess() {
     return _int_80(PRINT_ALL_PROCESS, 0, 0, 0);
}

void terminateProcess(uint64_t pid) {
     _int_80(TERMINATE_PROCESS_BY_ID, pid, 0, 0);
}
