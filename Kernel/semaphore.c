#include <semaphore.h>

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

static char *getMutexId(char *semaphoreId);
static uint32_t existSemaphore(char *semaphoreId);
static uint64_t dequeueProcessId(listObject_t processQueue);
static int semaphoreCompare(char *semaphoreId, semaphore_t *semaphore);

static listObject_t semaphores;

int semaphoreWait(char *semaphoreId, uint64_t processId) {
	if(!existSemaphore(semaphoreId)) {
		return ERROR_STATE;
	}

	semaphore_t *semaphore = getSemaphore(semaphoreId);

	lock(semaphore->mutex, processId);

	if(semaphore->counter == INT_MIN) {
		return ERROR_STATE;
	}

	semaphore->counter--;

	if(semaphore->counter < 0) {
		addElement(semaphore->sleepingProcessesId, (void *) &processId,
				   sizeof(uint64_t)); /* Adds pid to sleepProcessId */
		unlock(semaphore->mutex, processId);
		sleepCurrent(PROCESS_BLOCKED); /* Sleeps process */
	}

	return OK_STATE;
}

int semaphoreTryWait(char *semaphoreId, uint64_t processId) {
	if(!existSemaphore(semaphoreId)) {
		return ERROR_STATE;
	}

	semaphore_t *semaphore = getSemaphore(semaphoreId);

	lock(semaphore->mutex, processId);

	if(semaphore->counter > 0) {
		semaphore->counter--;
		unlock(semaphore->mutex, processId);
		return TRUE;
	}

	unlock(semaphore->mutex, processId);

	return FALSE;
}

int semaphorePost(char *semaphoreId, uint64_t processId) {
	if(!existSemaphore(semaphoreId)) {
		return ERROR_STATE;
	}

	semaphore_t *semaphore = getSemaphore(semaphoreId);

	if(semaphore->counter == INT_MAX) {
		return ERROR_STATE;
	}

	lock(semaphore->mutex, processId);

	if(semaphore->counter < 0 && size(semaphore->sleepingProcessesId) > 0) {
		uint64_t processId = dequeueProcessId(semaphore->sleepingProcessesId);
		wakeUp(processId);
	}

	semaphore->counter++;

	unlock(semaphore->mutex, processId);

	return OK_STATE;
}

void initSemaphores(uint64_t processId) {
	static int initializations = 0;

	if(initializations > 0) {
		return;
	}

	initializations++;
	semaphores = newList();
	createMutualExclusion(MUTEX_SEMAPHORE_MASTER_ID, processId);
}

int createSemaphore(char *semaphoreId, int counterInitialValue, uint64_t processId) {
	lock(MUTEX_SEMAPHORE_MASTER_ID, processId); /* For atomic creation */

	if(existSemaphore(semaphoreId)) {
		unlock(MUTEX_SEMAPHORE_MASTER_ID, processId);
		return ERROR_STATE;
	}

	char *mutexId = getMutexId(semaphoreId);
	if(createMutualExclusion(mutexId, processId) == ERROR_STATE) {
		return ERROR_STATE;
	}

	semaphore_t semaphore;
	semaphore.id 					= semaphoreId;
	semaphore.counter				= counterInitialValue;
	semaphore.mutex 				= mutexId;
	semaphore.sleepingProcessesId	= newList();

	addElement(semaphores, (void *) &semaphore, sizeof(semaphore_t));

	unlock(MUTEX_SEMAPHORE_MASTER_ID, processId);

	return OK_STATE;
}

int terminateSemaphore(char *semaphoreId, uint64_t processId) {
	lock(MUTEX_SEMAPHORE_MASTER_ID, processId); /* For atomic termination */

	if(!existSemaphore(semaphoreId)) {
		unlock(MUTEX_SEMAPHORE_MASTER_ID, processId);
		return ERROR_STATE;
	}

	removeSemaphore(semaphoreId);

	unlock(MUTEX_SEMAPHORE_MASTER_ID, processId);

	return OK_STATE;
}

static void removeSemaphore(char *semaphoreId) {
	semaphore_t *semaphore = getSemaphore(semaphoreId);
	removeAndFreeAllElements(semaphore->sleepingProcessesId);
	freeList(semaphore->sleepingProcessesId);
	removeAndFreeFirstElementByCriteria(semaphores, &semaphoreCompare,
		 								semaphoreId);
}

static semaphore_t *getSemaphore(char *semaphoreId) {
	return (semaphore_t *) getFirstElementReferenceByCriteria(semaphores,
		   &semaphoreCompare, semaphoreId);
}

static char *getMutexId(char *semaphoreId) {
	return stringConcatenation(MUTEX_ID_PREFIX, semaphoreId);
}

static uint32_t existSemaphore(char *semaphoreId) {
	return contains(semaphores, &semaphoreCompare, semaphoreId);
}

static uint64_t dequeueProcessId(listObject_t processQueue) {
	uint64_t processId;
	getFirstElement(processQueue, &processId);
	removeFirst(processQueue);
	return processId;
}

static int semaphoreCompare(char *semaphoreId, semaphore_t *semaphore) {
	return stringCompare(semaphoreId, semaphore->id);
}
