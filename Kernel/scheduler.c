#include <scheduler.h>

typedef struct scheduler_t {
   processControlBlockListPtr_t ready;
   processControlBlockListPtr_t waiting;
} scheduler_t;

#define SCHEDULER_MUTEX_ID "schedulerMutex"

void _cli(void);
void _sti(void);
void _context_switch(void);
void _force_context_switch(void);

static uint8_t ticksPassed = 0;
static scheduler_t scheduler;

void initializeScheduler() {
    scheduler.ready = initializePCBList();
    scheduler.waiting = initializePCBList();
    createMutualExclusion(SCHEDULER_MUTEX_ID);
    lockIfUnlocked(SCHEDULER_MUTEX_ID, 0);
}

void startScheduler() {
    //unlock(SCHEDULER_MUTEX_ID, 0);
}

void * schedule(void * currentProcessStackPointer) {;
	ticksPassed ++;
    void * aux;
    printWithColor("scheduleando\n",13,40); //evans
	if(ticksPassed == QUANTUM)  {
        ticksPassed = 0;
        nextProcess(currentProcessStackPointer);
        aux = getStackPointer(consultFirstPCBFromList(scheduler.ready));
    }
    else {
        aux = currentProcessStackPointer;
    }
	return aux;
}

void nextProcess(void * currentProcessStackPointer) {
    processControlBlockPtr_t pcb = removeFirstPCBFromList(scheduler.ready);
    setStackPointer(pcb, currentProcessStackPointer);
    if(isTerminate(pcb) == FALSE) {
        addProcessToScheduler(pcb);
    }
}

void addProcessToScheduler(processControlBlockPtr_t pcb) {
    if(isReady(pcb)) {
        addPCBToList(scheduler.ready, pcb);
    }
	else if (isWaiting(pcb)) {
        addPCBToList(scheduler.waiting, pcb);
    }
}

void terminateCurrentProcess() {
    printWithColor("terminate process\n", 18, 30); //evans
    processControlBlockPtr_t currentPCB = consultFirstPCBFromList(scheduler.ready);
    giveChildsToFather(currentPCB);
    setState(currentPCB, PROCESS_TERMINATE);
    _force_context_switch();
}

void sleepCurrent() {
    processControlBlockPtr_t currentPCB = consultFirstPCBFromList(scheduler.ready);
    setState(currentPCB, PROCESS_WAITING);
}

void wakeUp(uint64_t pid) {
    processControlBlockPtr_t pcb = PCBFromListByPID(scheduler.waiting, pid);
    if(pcb != NULL) {
        setState(pcb, PROCESS_READY);
        addProcessToScheduler(pcb);
    }
}
