#include <mutualExclusion.h>

int createMutualExclusion(char *mutexId) {
	return _int_80(CREATE_MUTEX, (uint64_t) mutexId, 0, 0);
}

int lock(char *mutexId) {
	return _int_80(LOCK_MUTEX, (uint64_t) mutexId, 0, 0);
}

int unlock(char *mutexId) {
	return _int_80(UNLOCK_MUTEX, (uint64_t) mutexId, 0, 0);
}

/* Return ERROR_STATE if mutexId not exist, TRUE if
   could lock and FALSE if was currently locked */
int lockIfUnlocked(char *mutexId) {
	return _int_80(LOCK_IF_UNLOCKED_MUTEX, (uint64_t) mutexId, 0, 0);
}

int terminateMutualExclusion(char *mutexId) {
	return _int_80(TERMINATE_MUTEX, (uint64_t) mutexId, 0, 0);
}
