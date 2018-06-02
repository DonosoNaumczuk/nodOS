#include <pipe.h>

/* Write and read are atomic, but independently.
   A process can read while other is writing, or viceversa.
   That synchronization holds on the user. */

/* What happen if isNonBlocking is TRUE? That means that if
   buffer is empty or full, you will get a the number of bytes
   that rest to write or EMPTY_BUFFER instead of beign blocked. */

typedef struct {
	char *id;
	uint8_t isNonBlocking;
	uint32_t byteSize;
	void *buffer;
	uint32_t readIndex; /* buffer index to read */
	uint32_t writeIndex; /* buffer index to write */
	listObject_t waitingForRead; /* sleeping until can read */
	listObject_t waitingForWrite; /* sleeping until can write */
	char *mutex; /* Mutex id for listObject_t atomic operations */
	char *writeMutex; /* Mutex id for atomic write operations */
	char *readMutex; /* Mutex id for atomic read operations */
	char *fullSemaphore; /* Semaphore id for full buffer block */
	char *emptySemaphore; /* Semaphore id for empty buffer block */
} pipe_t;

static listObject_t pipes;

void initPipe(uint64_t processId) {
	static int initializations = 0;

	if(initializations > 0) {
		return;
	}

	initializations++;

	pipes = newList();

	createMutualExclusion(MUTEX_PIPE_MASTER_ID, processId);
}

int createPipe(char *pipeId, uint32_t byteSize, uint32_t isNonBlocking,
	 		   uint64_t processId) {
	lock(MUTEX_PIPE_MASTER_ID, processId);

	if(existPipe(pipeId) || byteSize <= 0) {
		unlock(MUTEX_PIPE_MASTER_ID, processId);
		return ERROR_STATE;
	}

	pipe_t pipe;
	char *id						= pipeId;
	uint8_t isNonBlocking			= isNonBlocking;
	uint32_t byteSize				= byteSize;
	void *buffer					= allocateMemory(byteSize);
	uint32_t readIndex				= 0;
	uint32_t writeIndex				= 0;
	listObject_t waitingForRead		= newList();
	listObject_t waitingForWrite	= newList();
	char *mutex						= getMutexListId(pipeId);
	char *writeMutex				= getMutexWriteId(pipeId);
	char *readMutex					= getMutexReadId(pipeId);
	char *fullSemaphore				= getSemaphoreFullId(pipeId);
	char *emptySemaphore			= getSemaphoreEmptyId(pipeId);

	unlock(MUTEX_PIPE_MASTER_ID, processId);
}

int writeOnPipe(char *pipeId, void *data, uint32_t byteSize,
	 			uint64_t processId) {
	if(!existPipe(pipeId) || byteSize <= 0) {
		return ERROR_STATE;
	}

	pipe_t *pipe = getPipe(pipeId);

	lock(pipe->writeMutex, processId);

	for(uint32_t i = 0; i < byteSize; i++) {
		if(pipe->isNonBlocking) {
			if(!semaphoreTryWait(pipe->fullSemaphore, processId)) {
				unlock(pipe->writeMutex, processId);
				return byteSize - i;
			}
		}
		else {
			semaphoreWait(pipe->fullSemaphore, processId);
		}

		semaphorePost(pipe->emptySemaphore, processId);

		((char *) (pipe->buffer))[pipe->writeIndex] = ((char *) data)[i];

		pipe->writeIndex = (pipe->writeIndex + 1) % pipe->byteSize;
	}

	unlock(pipe->writeMutex, processId);

	return OK_STATE;
}

int readFromPipe(char *pipeId, void *buffer, uint32_t byteSize,
	 			 uint64_t processId) {
	if(!existPipe(pipeId) || byteSize <= 0) {
		return ERROR_STATE;
 	}

 	pipe_t *pipe = getPipe(pipeId);

 	lock(pipe->readMutex, processId);

	if(!semaphoreTryWait(pipe->emptySemaphore, processId)) {
		if(pipe->isNonBlocking) {
			unlock(pipe->readMutex, processId);
			return EMPTY_BUFFER;
		}
		else {
			semaphoreWait(pipe->emptySemaphore, processId);
		}
	}

	uint32_t i;

 	for(i = 0; i < byteSize; i++) {
		if(!semaphoreTryWait(pipe->emptySemaphore, processId)) {
			unlock(pipe->readMutex, processId);
			return i;
		}

 		semaphorePost(pipe->fullSemaphore, processId);

 		((char *) buffer)[i] = ((char *) (pipe->buffer))[pipe->readIndex];

 		pipe->readIndex = (pipe->readIndex + 1) % pipe->byteSize;
 	}

 	unlock(pipe->writeMutex, processId);

 	return i;
}

int terminatePipe(char *pipeId, uint64_t processId) {
	if(!existPipe(pipeId)) {
		return ERROR_STATE;
	}
}

static uint32_t existPipe(char *pipeId) {
	return contains(semaphores, &pipeCompare, semaphoreId);
}

static int pipeCompare(void *pipeId, void *pipe) {

}

static pipe_t *getPipe(char *pipeId);

static void removePipe(char *pipeId);

char *getMutexListId(char *pipeId) {

}

char *getMutexWriteId(char *pipeId) {

}

char *getMutexReadId(char *pipeId) {

}

char *getSemaphoreFullId(char *pipeId) {

}

char *getSemaphoreEmptyId(char *pipeId) {

}
