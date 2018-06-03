#ifndef _SCHEDULER_H_

#define _SCHEDULER_H_

#include <processControlBlockLib.h>
#include <stdint.h>
#include <memoryAllocator.h>
#include <videoDriver.h>
#include <mutualExclusion.h>
#include <processControlBlock.h>
#include <taskControlBlock.h>

#define SCHEDULER_MUTEX_ID "schedulerMutex"
#define SCHEDULER_PROCESS_ID 0
#define QUANTUM 1 /* Quantity of ticks to force context switching */

void initializeScheduler();
void startScheduler();
void * schedule(void * currentProcessStackPointer);
void nextTask(void * currentProcessStackPointer);
void addProcessToScheduler(taskControlBlockPtr_t tcb);
void terminateCurrentProcess(int returnValue);
void terminateAProcessByPid(uint64_t pid);
processControlBlockPtr_t getPCBByPid(uint64_t pid);
taskControlBlockPtr_t getTCBByTid(uint64_t tid);
int isBlocked(uint64_t tid);
void sleepCurrent(int condition);
void wakeUp(uint64_t tid);
int waitChild(uint64_t pid);
processControlBlockPtr_t getASonOfCurrentProcess();
processControlBlockPtr_t getCurrentPCB();
taskControlBlockPtr_t getCurrentTCB();
uint64_t getProcessId();
uint64_t getTaskId();
int isCurrentForeground();

#endif
