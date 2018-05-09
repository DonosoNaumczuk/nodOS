#ifndef _syscall_h_
#define _syscall_h_

#include <videoDriver.h>
#include <stdint.h>
#include <time.h>
#include <lib.h>
#include <read.h>
#include <scheduler.h>
#include <processControlBlock.h>
#include <messageQueue.h>
#include <mutualExclusion.h>
#include <semaphore.h>

/* Read and Write */
#define WRITE 					 0
#define READ 					 1

/* Write Pixel */
#define WRITE_PIXEL 			 2

/* Get Resolution */
#define GET_RESOLUTION_X		 3
#define GET_RESOLUTION_Y 		 4

/* Time */
#define TIME 					 5

/* Clear */
#define CLEAR 					 6

/* Process */
#define TERMINATE_PROCESS 		 7
#define SLEEP_PROCESS 			 8
#define WAIT_CHILD_PROCESS 		 9
#define WAKE_UP_PROCESS 		10
#define CREATE_PROCESS 			11
#define GET_PID 				12
#define PRINT_ALL_PROCESS       29
#define TERMINATE_PROCESS_BY_ID 30

/* Mailbox */
#define SEND_MAILBOX       		13
#define RECEIVE_MAILBOX    		14
#define CREATE_MAILBOX     		15
#define DESTROY_MAILBOX    		16

/* Memory Allocator */
#define ALLOCATE_MEMORY    		17
#define FREE               		18

/* Mutual Exclusion */
#define CREATE_MUTEX			19
#define LOCK_MUTEX				20
#define UNLOCK_MUTEX			21
#define LOCK_IF_UNLOCKED_MUTEX	22
#define TERMINATE_MUTEX			23

/* Semaphore */
#define CREATE_SEMAPHORE		24
#define SEMAPHORE_WAIT			25
#define SEMAPHORE_POST			26
#define SEMAPHORE_TRY_WAIT		27
#define TERMIANTE_SEMAPHORE		28

uint64_t syscall_dispatcher(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx);

#endif
