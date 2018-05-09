#ifndef _SCHEDULER_H_

#define _SCHEDULER_H_

#include <processControlBlockLib.h>
#include <stdint.h>
#include <memoryAllocator.h>
#include <videoDriver.h>
#include <mutualExclusion.h>
#include <processControlBlock.h>

#define SCHEDULER_MUTEX_ID "schedulerMutex"
#define SCHEDULER_PROCESS_ID 0
#define QUANTUM 1 /* Quantity of ticks to force context switching */

void initializeScheduler();
void * schedule(void * currentProcessStackPointer);
void addProcessToScheduler(processControlBlockPtr_t pcb);
void terminateCurrentProcess(int returnValue);
void nextProcess(void * currentProcessStackPointer);
void startScheduler();
void sleepCurrent(int condition);
void wakeUp(uint64_t pid);
int waitChild(uint64_t pid);
processControlBlockPtr_t getASonOfCurrentProcess();
uint64_t getProcessId();
processControlBlockPtr_t getCurrentPCB();
int isCurrentForeground();
void printAllProcess();

#endif
