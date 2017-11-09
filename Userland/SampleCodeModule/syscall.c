#include <syscall.h>

void write(char * string, uint64_t length, char color){
	_int_80(0,string,length,color);
}

//read()

void printPixel(uint32_t x, uint32_t y, char color){
	_int_80(2,x,y,color);
}

uint64_t getResolutionX(){
	return _int_80(3,0,0,0);
}

uint64_t getResolutionY(){
	return _int_80(4,0,0,0);
}

uint64_t getTime(int time){
	return _int_80(5,time,0,0);
}

void* malloc(int size){
	return _int_80(6,size,0,0);
}

void free(){
	_int_80(7,0,0,0);
}

void cleanScreen(){
	_int_80(8,0,0,0);
}