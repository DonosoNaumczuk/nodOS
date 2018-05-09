#ifndef SEMAPHORE_H

#define SEMAPHORE_H

#include <stdint.h>
#include <syscall.h>
#include <errors.h>
#include <bool.h>

int createSemaphore(char *semaphoreId, int counterInitialValue);

int semaphoreWait(char *semaphoreId);

int semaphorePost(char *semaphoreId);

/* Returns ERROR_STATE if semaphore not exists,
   TRUE if can decrement (without sleep/block)
   and FALSE otherwise. */
int semaphoreTryWait(char *semaphoreId);

int terminateSemaphore(char *semaphoreId);

#endif
