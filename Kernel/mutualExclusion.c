#include <mutualExclusion.h>

typedef struct {
	char *id;
	uint32_t status;
	sint64_t ownerProcessId;
	listObject_t sleepingProcessesId;
} mutex_t;

/*	Implemented at mutualExclusion.asm
	Set status to locked.
	Returns TRUE if was currenty locked,
	otherwise return FALSE, atomically. */
static uint32_t mutex_lock(uint32_t *status);
static uint32_t existMutex(char *mutexId);
static uint64_t dequeueProcessId(listObject_t processQueue);
static int mutexCompare(char *mutexId, mutex_t *mutex);
static mutex_t *getMutex(mutexId);
static void removeMutex(char *mutexId);

static listObject_t mutexes;

void initMutualExclusion() {
	static int initializations = 0;

	if(initializations > 0) {
		return;
	}

	initializations++;
	mutexes = newList();

	/* initialize mutex master */
	mutex_t mutex;
	mutex.id 					= MUTEX_MASTER_ID;
	mutex.status 				= UNLOCKED;
	mutex.ownerProcessId		= NULL_PID;
	mutex.sleepingProcessesId	= newList();

	addElement(mutexes, (void *) &mutex, sizeof(mutex_t));
}

int createMutualExclusion(char *mutexId, uint64_t processId) {
	lock(MUTEX_MASTER_ID, processId); /* For atomic mutex creation */

	if(existMutex(mutexId)) {
		unlock(MUTEX_MASTER_ID, processId);
		return ERROR_STATE;
	}

	mutex_t mutex;
	mutex.id 					= mutexId;
	mutex.status 				= UNLOCKED;
	mutex.ownerProcessId		= NULL_PID;
	mutex.sleepingProcessesId	= newList();

	addElement(mutexes, (void *) &mutex, sizeof(mutex_t));

	unlock(MUTEX_MASTER_ID, processId);

	return OK_STATE;
}

int lock(char *mutexId, uint64_t processId) {
	if(!existMutex(mutexId)) {
		return ERROR_STATE;
	}

	mutex_t *mutex = getMutex(mutexId);

    int wasLocked = mutex_lock(&mutex->status);

	if(wasLocked) {
		addElement(mutex->sleepingProcessesId, (void *) &processId,
				   sizeof(uint64_t)); /* Adds pid to sleepProcessId */
		sleepCurrent(PROCESS_BLOCKED); /* Sleeps process until mutex unlocked */
	}
	else {
		mutex->ownerProcessId = processId;
	}

	return OK_STATE;
}

int unlock(char *mutexId, uint64_t processId) {
	if(!existMutex(mutexId)) {
		return ERROR_STATE;
	}

	mutex_t *mutex = getMutex(mutexId);

	if(mutex->ownerProcessId == processId) {
		if(size(mutex->sleepingProcessesId) > 0) {
			uint64_t processId = dequeueProcessId(mutex->sleepingProcessesId);

			/* Mutex will still locked but now the owner is other process.
			   So only that process can unlock the mutex and any other process
			   who try to lock the mutex will fall to sleep.
			   So cool, isn't it? ;) */
			mutex->ownerProcessId = processId;
			wakeUp(processId);
		}
		else {
			mutex->status = UNLOCKED;
		}

		return OK_STATE;
	}

	return ERROR_STATE;
}

int lockIfUnlocked(char *mutexId, uint64_t processId) {
	if(!existMutex(mutexId)) {
		return ERROR_STATE;
	}

	mutex_t *mutex = getMutex(mutexId);

	int wasLocked = mutex_lock(&mutex->status);

	int couldLock = FALSE;

	if(!wasLocked) {
		couldLock = TRUE;
		mutex->ownerProcessId = processId;
	}

	return couldLock;
}

int terminateMutualExclusion(char *mutexId, uint64_t processId) {
	lock(MUTEX_MASTER_ID, processId); /* For atomic termination */

	if(!existMutex(mutexId)) {
		unlock(MUTEX_MASTER_ID, processId);
		return ERROR_STATE;
	}

	removeMutex(mutexId);

	unlock(MUTEX_MASTER_ID, processId);

	return OK_STATE;
}

static uint32_t existMutex(char *mutexId) {
	return contains(mutexes, &mutexCompare, mutexId);
}

static void removeMutex(char *mutexId) {
	mutex_t *mutex = getMutex(mutexId);
	removeAndFreeAllElements(mutex->sleepingProcessesId);
	freeList(mutex->sleepingProcessesId);
	removeAndFreeFirstElementByCriteria(mutexes, &mutexCompare, mutexId);
}

static mutex_t *getMutex(mutexId) {
	return (mutex_t *) getFirstElementReferenceByCriteria(mutexes,
		   &mutexCompare, mutexId);
}

static uint64_t dequeueProcessId(listObject_t processQueue) {
	uint64_t processId;
	getFirstElement(processQueue, &processId);
	removeAndFreeFirst(processQueue);
	return processId;
}

static int mutexCompare(char *mutexId, mutex_t *mutex) {
	return stringCompare(mutexId, mutex->id);
}
