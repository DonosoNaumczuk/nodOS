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
static int isFirst = TRUE;

void initializeScheduler() {
    scheduler.ready = initializePCBList();
    scheduler.waiting = initializePCBList();
    createMutualExclusion(SCHEDULER_MUTEX_ID);
    lockIfUnlocked(SCHEDULER_MUTEX_ID, 0);
}

void startScheduler() {
    unlock(SCHEDULER_MUTEX_ID, 0);
    _force_context_switch();
}

void * schedule(void * currentProcessStackPointer) {
	ticksPassed ++;
    void * aux = currentProcessStackPointer;

	if(ticksPassed == QUANTUM) {
        ticksPassed = 0;
    }
    if(ticksPassed == 0 && lockIfUnlocked(SCHEDULER_MUTEX_ID, 0))  {
        if(isFirst) {
            isFirst = FALSE;
        }
        else {
            nextProcess(currentProcessStackPointer);
        }
        aux = getStackPointer(consultFirstPCBFromList(scheduler.ready));
        unlock(SCHEDULER_MUTEX_ID, 0);
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

void terminateCurrentProcess(int returnValue) {
    processControlBlockPtr_t currentPCB = consultFirstPCBFromList(scheduler.ready);
    processControlBlockPtr_t currentPCBFather = getFather(currentPCB);
    giveChildsToFather(currentPCB);
    if(isWaiting(currentPCBFather)) {
        wakeUp(getPid(getFather(currentPCB)));
    }
    setReturnValue(currentPCB, returnValue);
    freeStack(currentPCB);
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

int waitChild(uint64_t pid) {
    processControlBlockPtr_t father = consultFirstPCBFromList(scheduler.ready);
    processControlBlockPtr_t son = PCBFromListByPID(getSons(father), pid);
    if(son != NULL) {
        while (!isTerminate(son)) {
            sleepCurrent();
        }
    }
    freeMemory(son);
    return getReturnValue(son);
}

processControlBlockPtr_t getASonOfCurrentProcess() {
    processControlBlockPtr_t current = consultFirstPCBFromList(scheduler.ready);
    processControlBlockListPtr_t currentSons = getSons(current);
    return consultFirstPCBFromList(currentSons);
}

processControlBlockPtr_t getCurrentPCB() {
    return consultFirstPCBFromList(scheduler.ready);
}

uint64_t getProcessID() {
    return getPid(consultFirstPCBFromList(scheduler.ready));
}
