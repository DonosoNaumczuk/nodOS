#ifndef _PROCESS_CONTROL_BLOCK_H_

#define _PROCESS_CONTROL_BLOCK_H_

typedef struct processControlBlock_t * processControlBlockPtr_t;

#include <memoryAllocator.h>
#include <processControlBlockLib.h>
#include <scheduler.h>
#include <taskControlBlock.h>
#include <listADT.h>

#define PROCESS_READY 0
#define PROCESS_TERMINATE 1
#define PROCESS_NEW 2
#define PROCESS_WAITING 3
#define PROCESS_BLOCKED 4

processControlBlockPtr_t createProcess(processControlBlockPtr_t parent, void *codeAddress, int argsQuantity, void ** processArgs);
uint64_t addTaskToProcess(processControlBlockPtr_t pcb, void *codeAddress, int argsQuantity, void ** processArgs);
void * addMemoryToHeap(processControlBlockPtr_t pcb, uint64_t size);
int freeMemoryFromHeap(processControlBlockPtr_t pcb, void * address);
char *getWriteSource(processControlBlockPtr_t pcb);
char *getReadSource(processControlBlockPtr_t pcb);
void setForeground(processControlBlockPtr_t pcb);
int isForeground(processControlBlockPtr_t pcb);
uint64_t getProcessIdOf(processControlBlockPtr_t pcb);
processControlBlockListPtr_t getSons(processControlBlockPtr_t pcb);
taskControlBlockPtr_t getMainTask(processControlBlockPtr_t pcb);
processControlBlockPtr_t getFather(processControlBlockPtr_t pcb);
int isThisPid(processControlBlockPtr_t pcb, long int pid);
void giveChildsToFather(processControlBlockPtr_t pcb);
void setReturnValue(processControlBlockPtr_t pcb, int returnValue);
int getReturnValue(processControlBlockPtr_t son);
void terminateAProcess(int returnValue, processControlBlockPtr_t pcb);
void wakeUpAPCB(processControlBlockPtr_t pcb);
void removeTCBFromPCB(processControlBlockPtr_t pcb, taskControlBlockPtr_t tcb);
int isProcessTerminate(processControlBlockPtr_t pcb);
taskControlBlockPtr_t getTaskByTid(processControlBlockPtr_t pcb, uint64_t tid);
void setReadSource(processControlBlockPtr_t pcb, char *readSource);
void setWriteSource(processControlBlockPtr_t pcb, char *writeSource);
void printPCB(processControlBlockPtr_t pcb);

#endif
