#include <syscall.h>

#define WRITE 0
#define READ 1
#define WRITE_PIXEL 2
#define GET_RESOLUTION_X 3
#define GET_RESOLUTION_Y 4
#define TIME 5
#define RESERVE 6
#define FREE 7
#define CLEAR 8

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
			return timeHandler(rdi);
		case RESERVE:
			return reserve(rdi);
		case FREE:
			free();
			return 0;
		case CLEAR:
			clear();
			return 0;
	}
}