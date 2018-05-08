#include <syscall.h>

uint64_t syscall_dispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	switch(rdi) {
		case WRITE:
			printWithColor((char*)rsi,rdx,(char)rcx);
			return 0;
		case READ:
			read((int)rsi, (unsigned char*)rdx, (int)rcx);
			return 0;
		case WRITE_PIXEL:
			printPixel((uint32_t)rsi,(uint32_t)rdx,(char)rcx);
			return 0;
		case GET_RESOLUTION_X:
			return getXResolution();
		case GET_RESOLUTION_Y:
			return getYResolution();
		case TIME:
			return timeHandler(rsi);
		case CLEAR:
			clear();
			return 0;
		case TERMINATE_PROCESS:
			terminateCurrentProcess(0);
			return 0;
		case SLEEP_PROCESS:
			sleepCurrent(rsi);
			return 0;
		case WAIT_CHILD_PROCESS:
			return waitChild(rsi);
		case WAKE_UP_PROCESS:
			wakeUp(rsi);
			return 0;
		case CREATE_PROCESS: {
			processControlBlockPtr_t parent = getCurrentPCB();
			processControlBlockPtr_t child = createProcess(parent, rsi, rdx, rcx);
			return getPid(child);
		}
		case GET_PID:
			return getProcessID();
		case SEND_MAILBOX:
			send(rsi, rdx, rcx);
			return 0;
		case RECEIVE_MAILBOX:
			return receive(rsi);
		case CREATE_MAILBOX:
			return createMailbox(rsi);
		case DESTROY_MAILBOX:
			closeMailbox(rsi);
			return 0;
		case ALLOCATE_MEMORY:
			return allocateMemory(rsi);
		case FREE:
			return freeMemory(rsi);
	}
}
