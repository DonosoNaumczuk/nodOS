#ifndef PIPE_H

#define PIPE_H

#include <stdint.h>

#define EMPTY_BUFFER 			-2
#define NON_BLOCKING 			 1
#define WITH_BLOCKING 			 0

int createPipe(char *pipeId, uint32_t byteSize, uint8_t isNonBlocking);

int writeOnPipe(char *pipeId, void *data, uint32_t byteSize);

int readFromPipe(char *pipeId, void *buffer, uint32_t byteSize);

int terminatePipe(char *pipeId);

#endif
