#ifndef SEMAPHORE_H

#define SEMAPHORE_H


#include <stdint.h>
#include <errors.h>
#include <bool.h>
#include <lib.h>
#include <listADT.h>
#include <compareFunctions.h>
#include <memoryAllocator.h>
#include <limits.h>
#include <mutualExclusion.h>
#include <processControlBlock.h>

#define MUTEX_SEMAPHORE_MASTER_ID "__mutex_semaphore_master_id__"
#define MUTEX_ID_PREFIX "__mutex/__"

/* Semaphore initialize function must be after
mutual exclusion initialize function */
void initSemaphores();
int createSemaphore(char *semaphoreId, int counterInitialValue, uint64_t processId);
int semaphoreWait(char *semaphoreId, uint64_t processId);
int semaphorePost(char *semaphoreId, uint64_t processId);
/* Returns ERROR_STATE if semaphore not exists,
   TRUE if can decrement (without sleep/block)
   and FALSE otherwise. */
int semaphoreTryWait(char *semaphoreId, uint64_t processId);


#endif
