#ifndef MUTUAL_EXCLUSION_H

#define MUTUAL_EXCLUSION_H

#include <stdint.h>
#include <errors.h>
#include <bool.h>
#include <listADT.h>
#include <compareFunctions.h>
#include <memoryAllocator.h>

#define MUTEX_MASTER_ID "__mutex_master_id__"
#define UNLOCKED	0
#define LOCKED		1
#define NULL_PID   -1

typedef signed long long sint64_t;

void initMutualExclusion();

int createMutualExclusion(char *mutexId, uint64_t processId);

int lock(char *mutexId, uint64_t processId);

int unlock(char *mutexId, uint64_t processId);

/* Return ERROR_STATE if mutexId not exist, TRUE if
   could lock and FALSE if was currently locked */
int lockIfUnlocked(char *mutexId, uint64_t processId);

#endif
