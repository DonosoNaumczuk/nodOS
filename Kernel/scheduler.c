#include <scheduler.h>

typedef struct scheduler_t {
   processControlBlockListPtr_t ready;
   processControlBlockListPtr_t waiting;
} scheduler_t;

static uint8_t ticksPassed = 0;
static scheduler_t scheduler;
static isInitialize = FALSE;
static isFirst = TRUE;

void initializeScheduler() {
    scheduler.ready = initializePCBList();
    scheduler.waiting = initializePCBList();
}

void startScheduler() {
    isInitialize = TRUE;
}

void * schedule(void * currentProcessStackPointer) {
	ticksPassed ++;
    void * aux;
	if(ticksPassed == QUANTUM)  {
        ticksPassed = 0;
    }
    if(ticksPassed == 0 && isInitialize) {
        if(isFirst){
            isFirst = FALSE;
        }
        else {
            nextProcess(currentProcessStackPointer);
        }
        newLine();
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
    processControlBlockPtr_t currentPCB = consultFirstPCBFromList(scheduler.ready);
    giveChildsToFather(currentPCB);
    setState(currentPCB, PROCESS_TERMINATE);
    nextProcess(NULL);
}

void sleepCurrent() {
    processControlBlockPtr_t currentPCB = consultFirstPCBFromList(scheduler.ready);
    setState(currentPCB, PROCESS_WAITING);
}

void wakeUp(int pid) {
    processControlBlockPtr_t pcb = PCBFromListByPID(scheduler.waiting, pid);
    if(pcb != NULL) {
        setState(pcb, PROCESS_READY);
        addProcessToScheduler(pcb);
    }
}
