#include <pipe.h>

int createPipe(char *pipeId, uint32_t byteSize, uint8_t isNonBlocking) {
     return _int_80(CREATE_PIPE, (uint64_t) pipeId, byteSize, isNonBlocking);
}

int writeOnPipe(char *pipeId, void *data, uint32_t byteSize) {
     return _int_80(WRITE_PIPE, (uint64_t) pipeId, (uint64_t) data, byteSize);
}

int readFromPipe(char *pipeId, void *buffer, uint32_t byteSize) {
     return _int_80(READ_PIPE, (uint64_t) pipeId, (uint64_t) buffer, byteSize);
}

int terminatePipe(char *pipeId) {
     return _int_80(TERMINATE_PIPE, (uint64_t) pipeId, 0, 0);
}
