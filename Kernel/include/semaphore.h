#ifndef SEMAPHORE_H

#define SEMAPHORE_H


#include <stdint.h>
#include <errors.h>
#include <bool.h>
#include <lib.h>
#include <listADT.h>
#include <compareFunctions.h>
#include <memoryAllocator.h>

#define MUTEX_SEMAPHORE_MASTER_ID "__mutex_semaphore_master_id__"
#define MUTEX_ID_PREFIX "__mutex/__"

/* Semaphore initialize function must be after
mutual exclusion initialize function */
void initSemaphores();
int createSemaphore(char *semaphoreId, int counterInitialValue);
int semaphoreWait(char *semaphoreId);
int semaphorePost(char *semaphoreId);
/* Returns ERROR_STATE if semaphore not exists,
   TRUE if can decrement (without sleep/block)
   and FALSE otherwise. */
int semaphoreTryWait(char *semaphoreId);


#endif
