#include <syscall.h>

#define WRITE 0
#define READ 1

void syscall_dispatcher(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx) {
	int i = rax;
	switch(i){
		case WRITE:
			printWithColor((char*)rbx,rcx,(char)rdx);
			break;
		case READ:
		;

		/*case WRITE_PIXEL:
			
		case GET_RESOLUTION_X:
		case GET_RESOLUTION_Y:
		case TIME:*/
	}
}