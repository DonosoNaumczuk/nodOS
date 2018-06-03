#include <pipe.h>

typedef struct {
	char *id;
	uint32_t status;
	sint64_t ownerProcessId;
	listObject_t sleepingProcessesId;
} mutex_t;

typedef struct {
	char *id;
	int counter;
	char *mutex;
	listObject_t sleepingProcessesId;
} semaphore_t;

/* Write and read are atomic, but independently.
A process can read while other is writing, or viceversa.
That synchronization holds on the user.

What happen if isNonBlocking is TRUE? That means that if
buffer is empty or full, you will get a the number of bytes
that rest to write or EMPTY_BUFFER instead of beign blocked. */

typedef struct {
	char *id;
	uint8_t isNonBlocking;
	uint32_t byteSize;
	void *buffer;
	uint32_t readIndex; /* buffer index to read */
	uint32_t writeIndex; /* buffer index to write */
	char *writeMutex; /* Mutex id for atomic write operations */
	char *readMutex; /* Mutex id for atomic read operations */
	char *fullSemaphore; /* Semaphore id for full buffer block */
	char *emptySemaphore; /* Semaphore id for empty buffer block */
} pipe_t;

static uint32_t existPipe(char *pipeId);
static int pipeCompare(char *pipeId, pipe_t *pipe);
static pipe_t *getPipe(char *pipeId);
static void removePipe(char *pipeId, uint64_t processId);
static char *getMutexListId(char *pipeId);
static char *getMutexWriteId(char *pipeId);
static char *getMutexReadId(char *pipeId);
static char *getSemaphoreFullId(char *pipeId);
static char *getSemaphoreEmptyId(char *pipeId);

static listObject_t pipes;

void initPipes(uint64_t processId) {
	static int initializations = 0;

	if(initializations > 0) {
		return;
	}

	initializations++;

	pipes = newList();

	createMutualExclusion(MUTEX_PIPE_MASTER_ID, processId);
}

int createPipe(char *pipeId, uint32_t byteSize, uint8_t isNonBlocking,
	 		   uint64_t processId) {
	lock(MUTEX_PIPE_MASTER_ID, processId);

	if(existPipe(pipeId) || byteSize <= 0) {
		unlock(MUTEX_PIPE_MASTER_ID, processId);
		return ERROR_STATE;
	}

	pipe_t pipe;
	pipe.id					= pipeId;
	pipe.isNonBlocking		= isNonBlocking;
	pipe.byteSize			= byteSize;
	pipe.buffer				= allocateMemory(byteSize);
	pipe.readIndex			= 0;
	pipe.writeIndex			= 0;
	pipe.writeMutex			= getMutexWriteId(pipeId);
	pipe.readMutex			= getMutexReadId(pipeId);
	pipe.fullSemaphore		= getSemaphoreFullId(pipeId);
	pipe.emptySemaphore		= getSemaphoreEmptyId(pipeId);

	createMutualExclusion(pipe.writeMutex, processId);
	createMutualExclusion(pipe.readMutex, processId);
	createSemaphore(pipe.fullSemaphore, pipe.byteSize, processId);
	createSemaphore(pipe.emptySemaphore, 0, processId);

	addElement(pipes, (void *) &pipe, sizeof(pipe_t));

	unlock(MUTEX_PIPE_MASTER_ID, processId);

	return OK_STATE;
}

int writeOnPipe(char *pipeId, void * data, uint32_t byteSize,
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

int readFromPipe(char *pipeId, void * buffer, uint32_t byteSize,
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

	uint32_t i = 0;

	do {
		semaphorePost(pipe->fullSemaphore, processId);

 		((char *) buffer)[i++] = ((char *) (pipe->buffer))[pipe->readIndex];

 		pipe->readIndex = (pipe->readIndex + 1) % pipe->byteSize;
	} while(i < byteSize && semaphoreTryWait(pipe->emptySemaphore, processId));

	unlock(pipe->readMutex, processId);

	return i;
}

static uint32_t existPipe(char *pipeId) {
	return contains(pipes, (int (*)(const void * , const void * )) &pipeCompare, pipeId);
}

static int pipeCompare(char *pipeId, pipe_t *pipe) {
	return stringCompare(pipeId, pipe->id);
}

static pipe_t *getPipe(char *pipeId) {
	return (pipe_t *) getFirstElementReferenceByCriteria(pipes,
		   (int (*)(const void *, const void *)) &pipeCompare,
		    pipeId);
}

int terminatePipe(char *pipeId, uint64_t processId) {
	lock(MUTEX_PIPE_MASTER_ID, processId);

	if(!existPipe(pipeId)) {
		unlock(MUTEX_PIPE_MASTER_ID, processId);
		return ERROR_STATE;
	}

	removePipe(pipeId, processId);

	unlock(MUTEX_PIPE_MASTER_ID, processId);

	return OK_STATE;
}

static void removePipe(char *pipeId, uint64_t processId) {
	pipe_t *pipe = getPipe(pipeId);

	freeMemory(pipe->buffer);

	terminateMutualExclusion(pipe->writeMutex, processId);
	terminateMutualExclusion(pipe->readMutex, processId);

	terminateSemaphore(pipe->fullSemaphore, processId);
	terminateSemaphore(pipe->emptySemaphore, processId);

	removeAndFreeFirstElementByCriteria(pipes, (int (*)(const void *, const void *)) &pipeCompare, pipeId);
}

static char *getMutexWriteId(char *pipeId) {
	return stringConcatenation(WRITE_MUTEX_PREFIX, pipeId);
}

static char *getMutexReadId(char *pipeId) {
	return stringConcatenation(READ_MUTEX_PREFIX, pipeId);
}

static char *getSemaphoreFullId(char *pipeId) {
	return stringConcatenation(FULL_SEM_PREFIX, pipeId);
}

static char *getSemaphoreEmptyId(char *pipeId) {
	return stringConcatenation(EMPTY_SEM_PREFIX, pipeId);
}
