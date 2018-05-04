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
    printWithColor("start\n", 5, 15); //evans
}

void * schedule(void * currentProcessStackPointer) {
    if(ticksPassed == 0 && isInitialize) {
        printWithColor("EEEE\n", 4, 15); //evans
    }
    printWithColor("scheduleando\n", 12, 15); // evans
    newLine();
	ticksPassed ++;
    void * aux;
    printHexa(currentProcessStackPointer); // evans
    newLine();
	if(ticksPassed == QUANTUM)  {
        ticksPassed = 0;
    }
    if(ticksPassed == 0 && isInitialize) {
        printWithColor("Chau\n", 4, 15); //evans
        if(isFirst){
            isFirst = FALSE;
        }
        else {
            nextProcess(currentProcessStackPointer);
        }
        printWithColor("Chau\n", 4, 15); //evans
            newLine();
        aux = getStackPointer(consultFirstPCBFromList(scheduler.ready));
    }
    else {
        aux = currentProcessStackPointer;
    }
    printHexa(aux); // evans
    newLine();
    newLine();

	return aux;
}

void nextProcess(void * currentProcessStackPointer) {
    processControlBlockPtr_t pcb = removeFirstPCBFromList(scheduler.ready);
    setStackPointer(pcb, currentProcessStackPointer);
    if(!isTerminate(pcb)) {
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
