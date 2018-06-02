#include <semaphore.h>
#include <error.h>

int createSemaphore(char *semaphoreId, int counterInitialValue) {
	return _int_80(CREATE_SEMAPHORE, (uint64_t) semaphoreId, counterInitialValue, 0);
}

int semaphoreWait(char *semaphoreId) {
	return _int_80(SEMAPHORE_WAIT, (uint64_t) semaphoreId, 0 , 0);
}

int semaphorePost(char *semaphoreId) {
	return _int_80(SEMAPHORE_POST, (uint64_t) semaphoreId, 0, 0);
}

/* Returns ERROR_STATE if semaphore not exists,
   TRUE if can decrement (without sleep/block)
   and FALSE otherwise. */
int semaphoreTryWait(char *semaphoreId) {
	return _int_80(SEMAPHORE_TRY_WAIT, (uint64_t) semaphoreId, 0, 0);
}

int terminateSemaphore(char *semaphoreId) {
	return _int_80(TERMIANTE_SEMAPHORE, (uint64_t) semaphoreId, 0, 0);
}
