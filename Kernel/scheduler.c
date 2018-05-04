#include <scheduler.h>

typedef struct scheduler_t {
   processControlBlockListPtr_t ready;
   processControlBlockListPtr_t waiting;
} scheduler_t;

static uint8_t ticksPassed = 0;
static scheduler_t scheduler;
static isInitialize = FALSE; //evans: se podria cambiar por un mutex
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
        addPCBToList(scheduler.ready, pcb);
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
    //evans:  hacer que los hijos se vuelvan hijos del padre
    setState(consultFirstPCBFromList(scheduler.ready), PROCESS_TERMINATE);
    nextProcess(NULL);
}
