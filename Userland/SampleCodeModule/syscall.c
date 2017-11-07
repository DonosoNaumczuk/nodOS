#include <syscall.h>

write(char * string, uint64_t length, char color){
	_int_80(0,string,length,color);
}

//read()

printPixel(uint32_t x, uint32_t y, char color){
	_int_80(2,x,y,color);
}

getResolutionX(){
	_int_80(3,0,0,0);
}

getResolutionY(){
	_int_80(4,0,0,0);
}