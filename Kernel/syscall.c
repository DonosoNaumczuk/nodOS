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
			return getProcessIdOf(child);
		}
		case GET_PID:
			return getProcessId();
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
	 	case CREATE_MUTEX:
			return createMutualExclusion(rsi, getProcessId());
	 	case LOCK_MUTEX:
			return lock(rsi, getProcessId());
	 	case UNLOCK_MUTEX:
			return unlock(rsi, getProcessId());
	 	case LOCK_IF_UNLOCKED_MUTEX:
			return lockIfUnlocked(rsi, getProcessId());
	 	case TERMINATE_MUTEX:
			return terminateMutualExclusion(rsi, getProcessId());
	 	case CREATE_SEMAPHORE:
			return createSemaphore(rsi, rdx, getProcessId());
	 	case SEMAPHORE_WAIT:
			return semaphoreWait(rsi, getProcessId());
	 	case SEMAPHORE_POST:
			return semaphorePost(rsi, getProcessId());
	 	case SEMAPHORE_TRY_WAIT:
			return semaphoreTryWait(rsi, getProcessId());
	 	case TERMIANTE_SEMAPHORE:
			return terminateSemaphore(rsi, getProcessId());
	}
}
