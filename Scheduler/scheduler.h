typedef struct Scheduler {
    ProcessControlBlockList *ready;
    ProcessControlBlockList *waiting;
} Scheduler;

#define QUANTUM 1//quantity of ticks
void initializeScheduler();
void * schedule(void * currentProcessStackPointer);
void * addProcessToScheduler(ProcessControlBlock_t *pcb);
