#include <semaphore.h>

int createSemaphore(char *semaphoreId, int counterInitialValue) {
	return _int_80(CREATE_SEMAPHORE, semaphoreId, counterInitialValue, 0);
}

int semaphoreWait(char *semaphoreId) {
	return _int_80(SEMAPHORE_WAIT, semaphoreId, 0 , 0);
}

int semaphorePost(char *semaphoreId) {
	return _int_80(SEMAPHORE_POST, semaphoreId, 0, 0);
}

/* Returns ERROR_STATE if semaphore not exists,
   TRUE if can decrement (without sleep/block)
   and FALSE otherwise. */
int semaphoreTryWait(char *semaphoreId) {
	return _int_80(SEMAPHORE_TRY_WAIT, semaphoreId, 0, 0);
}

int terminateSemaphore(char *semaphoreId) {
	return _int_80(TERMIANTE_SEMAPHORE, semaphoreId, 0, 0);
}
