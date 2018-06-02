#ifndef _TASK_CONTROL_BLOCK_H_

#define _TASK_CONTROL_BLOCK_H_

typedef struct taskControlBlock_t * taskControlBlockPtr_t;

#include <memoryAllocator.h>
#include <scheduler.h>
#include <processControlBlock.h>

#define TASK_READY 0
#define TASK_TERMINATE 1
#define TASK_NEW 2
#define TASK_WAITING 3
#define TASK_BLOCKED 4

#define SIZE_OF_STACK 4096 /* 4K */

taskControlBlockPtr_t createTask(processControlBlockPtr_t pcb, void *codeAddress, int argsQuantity, void ** processArgs);
taskControlBlockPtr_t initializeTCB(processControlBlockPtr_t pcb, void *codeAddress, int argsQuantity, void ** processArgs) {
void setStackPointer(taskControlBlockPtr_t tcp, void * stackPointer) {
void * getStackPointer(taskControlBlockPtr_t tcp);
int isTerminate(taskControlBlockPtr_t tcb);
int isWaiting(taskControlBlockPtr_t tcb);
int isBlockedByTCB(taskControlBlockPtr_t tcb);
int isReady(taskControlBlockPtr_t tcb);
int isMainTask(taskControlBlockPtr_t tcb);
processControlBlockPtr_t getPCBOf(taskControlBlockPtr_t tcb);
uint64_t getTaskIdOf(taskControlBlockPtr_t tcb);
void setState(taskControlBlockPtr_t tcb, int state);
void terminateATask(taskControlBlockPtr_t tcb);
void startProcess(int argsQuantity, void ** processArgs, void * codeAddress);

#endif
