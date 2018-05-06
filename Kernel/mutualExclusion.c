#include <mutualExclusion.h>

typedef struct {
	char *id;
	uint8_t status;
	sint64_t ownerProcessId;
	listObject_t sleepingProcessesId;
} mutex_t;

static uint8_t existMutex(char *mutexId);
static uint64_t dequeueProcessId(listObject_t processQueue);
static uint8_t mutex_lock(uint8_t status); /* At mutualExclusion.asm */

static listObject_t mutexes;

int initMutualExclusion(char *mutexId) {
	if(existMutex(mutexId)) {
		return ERROR_STATE;
	}

	mutex_t *mutex 				= allocateMemory(sizeof(mutex_t));
	mutex->id 					= mutexId;
	mutex->status 				= UNLOCKED;
	mutex->ownerProcessId		= NULL_PID;
	mutex->sleepingProcessesId	= allocateMemory(sizeof(listObject_t));

	if(mutex == NULL || sleepingProcessesId == NULL) {
		return ERROR_STATE;
	}

	return OK_STATE;
}

int lock(char *mutexId, uint64_t processId) {
	if(!existMutex(mutexId)) {
		return ERROR_STATE;
	}

	mutex_t mutex;

	getFirstElementByCriteria(mutexes, &stringCompare, mutexId, (void *) &mutex);

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

	mutex_t mutex;

	getFirstElementByCriteria(mutexes, &stringCompare, mutexId, (void *) &mutex);

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

	mutex_t mutex;

	getFirstElementByCriteria(mutexes, &stringCompare, mutexId, (void *) &mutex);

	int wasLocked = mutex_lock(&mutex->status);

	int couldLock = FALSE;

	if(!wasLocked) {
		couldLock = TRUE;
		mutex->ownerProcessId = processId;
	}

	return couldLock;
}


static uint8_t existMutex(char *mutexId) {
	return contains(mutexes, &uintCompare, mutexId);
}

static uint64_t dequeueProcessId(listObject_t processQueue) {
	uint64_t processId;
	getFirstElement(processQueue, &processId);
	removeFirst(processQueue);
	return processId;
}
