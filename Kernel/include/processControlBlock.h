#ifndef _PROCESS_CONTROL_BLOCK_H_

#define _PROCESS_CONTROL_BLOCK_H_

typedef struct processControlBlock_t * processControlBlockPtr_t;

#include <memoryAllocator.h>
#include <processControlBlockLib.h>
#include <scheduler.h>
#include <taskControlBlock.h>

processControlBlockPtr_t createProcess(processControlBlockPtr_t parent, void *codeAddress, int argsQuantity, void ** processArgs);
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

#endif
