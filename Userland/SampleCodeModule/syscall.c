#include <syscall.h>

void write(char * string, uint64_t length, char color){
	_int_80(WRITE,string,length,color);
}

void read(int in_id, char * buffer, int qty_to_read) {
	_int_80(READ, in_id, buffer, qty_to_read);
}

void printPixel(uint32_t x, uint32_t y, char color){
	_int_80(WRITE_PIXEL,x,y,color);
}

uint64_t getResolutionX(){
	_int_80(GET_RESOLUTION_X,0,0,0);
}

uint64_t getResolutionY(){
	_int_80(GET_RESOLUTION_Y,0,0,0);
}

uint64_t getTime(int time){
	_int_80(TIME,time,0,0);
}

void* malloc(int size){
	_int_80(RESERVE,size,0,0);
}

void free(){
	_int_80(FREE,0,0,0);
}

void cleanScreen(){
	_int_80(CLEAR,0,0,0);
}