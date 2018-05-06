#include <listADT.h>

#define UNLOCKED 0
#define LOCKED 1
#define NULL_PID 0

typedef struct {
	char *id;
	uint8_t status;
	uint64_t ownerProcessId;
	listObject_t sleepingProcessesId;
} mutex_t;

int initMutualExlusion(char *mutexId);
int lock(char *mutexId, uint64_t processId);
int unlock(char *mutexId, uint64_t processId);
static uint8_t existMutex(char *mutexId);
static uint64_t dequeueProcessId(listObject_t processQueue);
static uint8_t mutex_lock(uint8_t status); /* mutualExlusion.asm */

static listObject_t mutexes;

int initMutualExlusion(char *mutexId) {
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

	mutex_t *mutex = (mutex_t *) getFirstElementByCriteria(mutexes, mutexId,
														   criteria);

    int wasLocked = mutex_lock(&mutex->status);

	if(wasLocked) {
		addElement(mutex->sleepingProcessesId, (void *) &getProcessId(),
				   sizeof(uint64_t)); /* adds pid to sleepProcessId */
		sleepCurrent(); /* sleep process */
	}
	else {
		mutex->ownerProcessId = getProcessId();
	}

	return OK_STATE;
}

int unlock(char *mutexId, uint64_t processId) {
	mutex_t *mutex = (mutex_t *) getFirstElementByCriteria(mutexes, mutexId,
		 			  criteria);

	if(existMutex(mutexId) && (mutex->ownerProcessId == processId)) {
		if(size(mutex->sleepingProcessesId) > 0) {
			uint64_t processId = dequeueProcessId(mutex->sleepingProcessesId);

			/* Mutex will still locked but now the owner is other process.
			   So only that process can unlock the mutex and any other process
			   who try to lock the mutex will fall to sleep. */
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

static uint8_t existMutex(char *mutexId) {
	return contains(mutexes, mutexId, criteria);
}

static uint64_t dequeueProcessId(listObject_t processQueue) {
	uint64_t processId;
	getFirstElement(processQueue, &processId);
	removeFirst(processQueue);
	return processId;
}
