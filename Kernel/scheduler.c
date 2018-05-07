#include <scheduler.h>

typedef struct scheduler_t {
   processControlBlockListPtr_t ready;
   processControlBlockListPtr_t waiting;
} scheduler_t;

#define SCHEDULER_MUTEX_ID "schedulerMutex"

void _force_context_switch(void);
int mutex_lock(uint8_t * arg);


static uint8_t ticksPassed = 0;
static scheduler_t scheduler;
static uint8_t mutex = 0;
static int isFirst = TRUE;

void initializeScheduler() {
    scheduler.ready = initializePCBList();
    scheduler.waiting = initializePCBList();
    createMutualExclusion(SCHEDULER_MUTEX_ID);
    mutex_lock(&mutex);
}

void startScheduler() {
    mutex = 0;
}

void * schedule(void * currentProcessStackPointer) {
	ticksPassed ++;
    void * aux = currentProcessStackPointer;

	if(ticksPassed == QUANTUM) {
        ticksPassed = 0;
    }
    if(ticksPassed == 0 && !mutex_lock(&mutex))  {
        if(isFirst) {
            isFirst = FALSE;
        }
        else {
            nextProcess(currentProcessStackPointer);
        }
        aux = getStackPointer(consultFirstPCBFromList(scheduler.ready));
        mutex = 0;
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
    processControlBlockPtr_t currentPCB = consultFirstPCBFromList(scheduler.ready);
    processControlBlockPtr_t currentPCBFather = getFather(currentPCB);
    giveChildsToFather(currentPCB);
    if(isWaiting(currentPCBFather)) {
        wakeUp(getFather(currentPCB));
    }
    setState(currentPCB, PROCESS_TERMINATE);
    _force_context_switch();
}

void sleepCurrent() {
    processControlBlockPtr_t currentPCB = consultFirstPCBFromList(scheduler.ready);
    setState(currentPCB, PROCESS_WAITING);
    _force_context_switch();
}

void wakeUp(uint64_t pid) {
    processControlBlockPtr_t pcb = PCBFromListByPID(scheduler.waiting, pid);
    if(pcb != NULL) {
        setState(pcb, PROCESS_READY);
        addProcessToScheduler(pcb);
    }
}

void wait(uint64_t pid, processControlBlockPtr_t father) {
    processControlBlockPtr_t son = PCBFromListByPID(father, pid);
    if(son != NULL) {
        while (!isTerminate(son)) {
            sleepCurrent();
        }
    }
}
