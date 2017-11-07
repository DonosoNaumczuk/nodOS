#include <syscall.h>

#define WRITE 0
#define READ 1
#define WRITE_PIXEL 2
#define GET_RESOLUTION_X 3
#define GET_RESOLUTION_Y 4
#define TIME 5

uint64_t syscall_dispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	switch(rdi){
		case WRITE:
			printWithColor((char*)rsi,rdx,(char)rcx);
			return 0;
		case READ:
			return;
		case WRITE_PIXEL:
			printPixel((uint32_t)rsi,(uint32_t)rdx,(char)rcx);
			return 0;
		case GET_RESOLUTION_X:
			return getXResolution();
		case GET_RESOLUTION_Y:
			return getYResolution();
		case TIME:
			return ;//nose para mi recibe un parametro que filtre entre segondos, horas, etc.
	}
}