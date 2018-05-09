#ifndef _PROCESS_CONTROL_BLOCK_H_

#define _PROCESS_CONTROL_BLOCK_H_

typedef struct processControlBlock_t * processControlBlockPtr_t;

#include <memoryAllocator.h>
#include <processControlBlockLib.h>
#include <scheduler.h>

#define PROCESS_READY 0
#define PROCESS_TERMINATE 1
#define PROCESS_NEW 2
#define PROCESS_WAITING 3
#define PROCESS_BLOCKED 4

#define SIZE_OF_STACK 4096 /* 4K */

processControlBlockPtr_t createProcess(processControlBlockPtr_t parent, void *codeAddress, int argsQuantity, void ** processArgs);
processControlBlockPtr_t initializePCB(processControlBlockPtr_t parent, void *codeAddress, int argsQuantity, void ** processArgs);
void * getStackPointer(processControlBlockPtr_t pcb);
uint64_t getProcessIdOf(processControlBlockPtr_t pcb);
processControlBlockPtr_t getFather(processControlBlockPtr_t pcb);
processControlBlockListPtr_t getSons(processControlBlockPtr_t pcb);
int isThisPid(processControlBlockPtr_t pcb, long int pid);
int isTerminate(processControlBlockPtr_t pcb);
int isWaiting(processControlBlockPtr_t pcb);
int isBlocked(processControlBlockPtr_t pcb);
int isReady(processControlBlockPtr_t pcb);
void startProcess(int argsQuantity, void ** processArgs, void * codeAddress);
void * startStack(void * codeAddress, void * stackBaseAddress, int argsQuantity,
                 void ** processArgs);
void freeStack(processControlBlockPtr_t pcb);
void setState(processControlBlockPtr_t pcb, int state);
void giveChildsToFather(processControlBlockPtr_t pcb);
int isThisPid(processControlBlockPtr_t pcb, long int pid);
void setStackPointer(processControlBlockPtr_t pcb, void * stackPointer);
void setReturnValue(processControlBlockPtr_t pcb, int returnValue);
void printPCB(processControlBlockPtr_t pcb);

#endif
