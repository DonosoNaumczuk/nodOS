#include <syscall.h>

uint64_t syscall_dispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	switch(rdi){
		case WRITE:
			printWithColor((char*)rsi,rdx,(char)rcx);
			return 0;
		case READ:
			read((int)rsi, (char*)rdx, (int)rcx);
			return 0;
		case WRITE_PIXEL:
			printPixel((uint32_t)rsi,(uint32_t)rdx,(char)rcx);
			return 0;
		case GET_RESOLUTION_X:
			return getXResolution();
		case GET_RESOLUTION_Y:
			return getYResolution();
		case TIME:
<<<<<<< HEAD
			return timeHandler(rsi);
		case RESERVE:
			return reserve(rdi);
		case FREE:
			free();
=======
			return timeHandler(rdi);
		case CLEAR:
			clear();
>>>>>>> ad42e7642eedefd498b8c87f98d99719f17aea94
			return 0;
	}
}
