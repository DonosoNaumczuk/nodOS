#ifndef _syscall_h_
#define _syscall_h_
#include <videoDriver.h>
#include <stdint.h>
#include <time.h>
#include <lib.h>
#include <read.h>

#define WRITE 0
#define READ 1
#define WRITE_PIXEL 2
#define GET_RESOLUTION_X 3
#define GET_RESOLUTION_Y 4
#define TIME 5
#define CLEAR 6

uint64_t syscall_dispatcher(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx);

#endif
