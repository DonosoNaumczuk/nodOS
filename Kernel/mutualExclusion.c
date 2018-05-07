#include <mutualExclusion.h>

typedef struct {
	char *id;
	uint8_t status;
	sint64_t ownerProcessId;
	listObject_t sleepingProcessesId;
} mutex_t;

static uint8_t existMutex(char *mutexId);
static uint64_t dequeueProcessId(listObject_t processQueue);
static int mutexCompare(char *mutexId, mutex_t *mutex);

/*	Implemented at mutualExclusion.asm
	Set status to locked.
	Returns TRUE if was currenty locked,
	otherwise return FALSE, atomically. */
static uint8_t mutex_lock(uint8_t *status);

static listObject_t mutexes;

void initMutualExclusion() {
	static int initializations = 0;

	if(initializations > 0) {
		return;
	}

	initializations++;
	mutexes = newList();
}

int createMutualExclusion(char *mutexId) {
	if(existMutex(mutexId)) {
		return ERROR_STATE;
	}

	mutex_t *mutex				= (mutex_t *) allocateMemory(sizeof(mutex_t));
	mutex->id 					= mutexId;
	mutex->status 				= UNLOCKED;
	mutex->ownerProcessId		= NULL_PID;
	mutex->sleepingProcessesId	= newList();

	addElement(mutexes, (void *) mutex, sizeof(mutex_t));

	return OK_STATE;
}

int lock(char *mutexId, uint64_t processId) {
	if(!existMutex(mutexId)) {
		return ERROR_STATE;
	}

	mutex_t *mutex = (mutex_t *) allocateMemory(sizeof(mutex_t));

	getFirstElementByCriteria(mutexes, &mutexCompare, mutexId, (void *) mutex);

    int wasLocked = mutex_lock(&mutex->status);

	if(wasLocked) {
		addElement(mutex->sleepingProcessesId, (void *) &processId,
				   sizeof(uint64_t)); /* Adds pid to sleepProcessId */
		sleepCurrent(); /* Sleeps process until mutex unlocked */
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

	mutex_t *mutex = (mutex_t *) allocateMemory(sizeof(mutex_t));

	getFirstElementByCriteria(mutexes, &mutexCompare, mutexId, (void *) mutex);

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

	mutex_t *mutex = (mutex_t *) allocateMemory(sizeof(mutex_t));

	getFirstElementByCriteria(mutexes, &mutexCompare, mutexId, (void *) mutex);
	printHexa(mutex->status);
	newLine();
	int wasLocked = mutex_lock(&mutex->status);
	printHexa(mutex->status);
	newLine();
	int couldLock = FALSE;

	if(!wasLocked) {
		couldLock = TRUE;
		mutex->ownerProcessId = processId;
	}

	return couldLock;
}


static uint8_t existMutex(char *mutexId) {
	return contains(mutexes, &mutexCompare, mutexId);
}

static uint64_t dequeueProcessId(listObject_t processQueue) {
	uint64_t processId;
	getFirstElement(processQueue, &processId);
	removeFirst(processQueue);
	return processId;
}

static int mutexCompare(char *mutexId, mutex_t *mutex) {
	return stringCompare(mutexId, mutex->id);
}
