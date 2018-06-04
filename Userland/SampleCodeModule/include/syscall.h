#ifndef _syscall_userland_h_
#define _syscall_userland_h_
#include <stdint.h>
#include <int80.h>

/* Read and Write */
#define WRITE 					 0
#define READ 					 1

/* Write Pixel */
#define WRITE_PIXEL 			 2

/* Get Resolution */
#define GET_RESOLUTION_X		      3
#define GET_RESOLUTION_Y 		 4

/* Time */
#define TIME_SYSCALL			 5

/* Clear */
#define CLEAR 					 6

/* Process */
#define TERMINATE_CURRENT_PROCESS   7
#define SLEEP_PROCESS 			 8
#define WAIT_CHILD_PROCESS 		 9
#define WAKE_UP_PROCESS 		     10
#define CREATE_PROCESS 			11
#define GET_PID 				12

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
#define CREATE_SEMAPHORE		     24
#define SEMAPHORE_WAIT			25
#define SEMAPHORE_POST			26
#define SEMAPHORE_TRY_WAIT		27
#define TERMIANTE_SEMAPHORE		28
/* ps*/
#define PRINT_ALL_PROCESS          29
#define TERMINATE_PROCESS_BY_ID    30

/* Pipes */
#define CREATE_PIPE                31
#define WRITE_PIPE                 32
#define READ_PIPE                  33
#define TERMINATE_PIPE             34

/*Read and Write Source*/
#define SET_READ_SOURCE            38
#define SET_WRITE_SOURCE           39


#define KEYBOARD_IN_ID			 0
#define SCREEN_IN_ID 		 	 1

#include <syscall.h>

void write(char * string, uint64_t length, char color);

void read(int in_id, char * buffer, int qty_to_read);

void printPixel(uint32_t x, uint32_t y, char color);

uint64_t getResolutionX();

uint64_t getResolutionY();

uint64_t getTime(int time);

void cleanScreen();

uint64_t terminateCurrentProcess();

uint64_t createProcess(void * codeAddress, uint32_t parametersQuantity, void ** parametersVector);

int sleepProcess();

int wakeUpProcess(uint64_t processId);

int waitChild(uint64_t processId);

int getPid();

void * allocateMemory(uint64_t bytesToAllocate);

uint32_t freeMemory(void * addressToFree);

int createMailbox(const char *mailboxId);

void send(const char *mailboxId, const void *message, const unsigned int messageSize);

void * receive(const char *mailboxId);

void closeMailbox(const char *mailboxId);

int printAllProcess();

void terminateProcess(uint64_t pid);

void setReadSource(char *readSource);

void setWriteSource(char *writeSource);

#endif
