#ifndef MUTUAL_EXCLUSION_H

#define MUTUAL_EXCLUSION_H

#include <stdint.h>
#include <syscall.h>
#include <bool.h>
#include <errors.h>

typedef signed long long sint64_t;

int createMutualExclusion(char *mutexId);

int lock(char *mutexId);

int unlock(char *mutexId);

/* Return ERROR_STATE if mutexId not exist, TRUE if
   could lock and FALSE if was currently locked */
int lockIfUnlocked(char *mutexId);

int terminateMutualExclusion(char *mutexId);

#endif
