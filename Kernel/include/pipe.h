#ifndef PIPE_H

#define PIPE_H

#include <stdint.h>
#include <mutualExclusion.h>
#include <semaphore.h>
#include <errors.h>

#define EMPTY_BUFFER 			-2
#define MUTEX_PIPE_MASTER_ID 	"__pipe__mutex__master__id__"
#define FULL_SEM_PREFIX 		"__pipe__full__sem__"
#define EMPTY_SEM_PREFIX  		"__pipe__empty__sem__"
#define WRITE_MUTEX_PREFIX 		"__pipe__write__mutex__"
#define READ_MUTEX_PREFIX 		"__pipe__read__mutex__"
#define LIST_MUTEX_PREFIX 		"__pipe__list__mutex__"

#define NON_BLOCKING 			 1
#define WITH_BLOCKING 			 0

void initPipes(uint64_t processId);
int createPipe(char *pipeId, uint32_t byteSize, uint8_t isNonBlocking,
	 		   uint64_t processId);
int writeOnPipe(char *pipeId, void *data, uint32_t byteSize,
	 			uint64_t processId);
int readFromPipe(char *pipeId, void *buffer, uint32_t byteSize,
	 			 uint64_t processId);
int terminatePipe(char *pipeId, uint64_t processId);

#endif
