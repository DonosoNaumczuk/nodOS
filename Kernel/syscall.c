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
			processControlBlockPtr_t child = createProcess(parent, (void *)rsi, rdx, (void **)rcx);
			return getProcessIdOf(child);
		}
		case GET_PID:
			return getProcessId();
		case SEND_MAILBOX:
			send((char*)rsi, (void *)rdx, rcx);
			return 0;
		case RECEIVE_MAILBOX:
			return (uint64_t)receive((char *)rsi);
		case CREATE_MAILBOX:
			return createMailbox((char *)rsi);
		case DESTROY_MAILBOX:
			closeMailbox((char*)rsi);
			return 0;
		case ALLOCATE_MEMORY:
			return (uint64_t)addMemoryToHeap(getCurrentPCB(), rsi);
		case FREE:
			return freeMemoryFromHeap(getCurrentPCB(), (void *)rsi);
	 	case CREATE_MUTEX:
			return createMutualExclusion((char *)rsi, getTaskId());
	 	case LOCK_MUTEX:
			return lock((char *)rsi, getTaskId());
	 	case UNLOCK_MUTEX:
			return unlock((char *)rsi, getTaskId());
	 	case LOCK_IF_UNLOCKED_MUTEX:
			return lockIfUnlocked((char *)rsi, getTaskId());
	 	case TERMINATE_MUTEX:
			return terminateMutualExclusion((char *)rsi, getTaskId());
	 	case CREATE_SEMAPHORE:
			return createSemaphore((char *)rsi, rdx, getTaskId());
	 	case SEMAPHORE_WAIT:
			return semaphoreWait((char *)rsi, getTaskId());
	 	case SEMAPHORE_POST:
			return semaphorePost((char *)rsi, getTaskId());
	 	case SEMAPHORE_TRY_WAIT:
			return semaphoreTryWait((char *)rsi, getTaskId());
	 	case TERMIANTE_SEMAPHORE:
			return terminateSemaphore((char *)rsi, getTaskId());
		case PRINT_ALL_PROCESS:
			printAllProcess();
			return 0;
		case TERMINATE_PROCESS_BY_ID:
			terminateAProcessByPid(rsi);
			return 0;
		case CREATE_PIPE:
			return createPipe((char *)rsi, (uint32_t) rdx, (uint8_t) rcx, getProcessId());
		case WRITE_PIPE:
			return writeOnPipe((char *)rsi, (void * ) rdx, (uint32_t) rcx, getProcessId());
		case READ_PIPE:
			return readFromPipe((char *)rsi, (void * ) rdx, (uint32_t) rcx, getProcessId());
		case TERMINATE_PIPE:
			return terminatePipe((char *)rsi, getProcessId());
		case CREATE_TASK:
			return addTaskToProcess(getCurrentPCB(), (void *)rsi, rdx, (void **)rcx);
		case TERMINATE_TASK:
			terminateATaskByTid(rsi);
			return 0;
		case WAIT_TASK:
			waitTask(rsi);
			return 0;
	}

	return 0;
}
