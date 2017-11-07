#include <syscall.h>

write(char * string, uint64_t length, char color){
	_int_80(0,string,length,color);
}