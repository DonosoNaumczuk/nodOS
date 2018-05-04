#include <scheduler.h>

typedef struct scheduler_t {
   processControlBlockListPtr_t ready;
   processControlBlockListPtr_t waiting;
} scheduler_t;

static uint8_t ticksPassed = 0;
static scheduler_t scheduler;
static isInitialize = FALSE; //evans: se podria cambiar por un mutex

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
	if(ticksPassed == QUANTUM && isInitialize) {
		ticksPassed = 0;
        aux = nextProcess(currentProcessStackPointer);
	}
    else {
        aux = currentProcessStackPointer;
    }
	return aux;
}

void * nextProcess(void * currentProcessStackPointer) {
    processControlBlockPtr_t pcb = removeFirstPCBFromList(scheduler.ready);
    setStackPointer(pcb, currentProcessStackPointer);
    if(!isTerminate(pcb)) {
        addPCBToList(scheduler.ready, pcb);
    }
    return getStackPointer(consultFirstPCBFromList(scheduler.ready));
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
    //evans:  hacer que los hijos se vuelvan hijos del padre
    setState(consultFirstPCBFromList(scheduler.ready), PROCESS_TERMINATE);
    nextProcess(NULL);
}
