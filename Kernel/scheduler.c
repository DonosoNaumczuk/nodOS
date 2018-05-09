#include <scheduler.h>

typedef struct scheduler_t {
   processControlBlockListPtr_t ready;
   processControlBlockListPtr_t waiting;
   processControlBlockListPtr_t terminated;
} scheduler_t;

void _force_context_switch(void);
int mutex_lock(uint8_t * arg);


static uint8_t ticksPassed = 0;
static scheduler_t scheduler;
static int isFirst = TRUE;
static processControlBlockPtr_t currentForegroundProcess = NULL;

void initializeScheduler() {
    scheduler.ready = initializePCBList();
    scheduler.waiting = initializePCBList();
    scheduler.terminated = initializePCBList();
    createMutualExclusion(SCHEDULER_MUTEX_ID, SCHEDULER_PROCESS_ID);
    lockIfUnlocked(SCHEDULER_MUTEX_ID, SCHEDULER_PROCESS_ID);
}

void setForeground(processControlBlockPtr_t pid) {
    currentForegroundProcess = pid;
}

uint64_t getForegroundPid() {
    return currentForegroundProcess == NULL ? 0 : getPid(currentForegroundProcess);
}

void startScheduler() {
    unlock(SCHEDULER_MUTEX_ID, SCHEDULER_PROCESS_ID);
    _force_context_switch();
}

void * schedule(void * currentProcessStackPointer) {
	ticksPassed ++;
    void * aux = currentProcessStackPointer;

	if(ticksPassed == QUANTUM) {
        ticksPassed = 0;
    }
    if(ticksPassed == 0 && lockIfUnlocked(SCHEDULER_MUTEX_ID, SCHEDULER_PROCESS_ID))  {
        if(isFirst) {
            isFirst = FALSE;
        }
        else {
            nextProcess(currentProcessStackPointer);
        }
        aux = getStackPointer(getCurrentPCB());
        unlock(SCHEDULER_MUTEX_ID, SCHEDULER_PROCESS_ID);
    }

	return aux;
}

void nextProcess(void * currentProcessStackPointer) {
    processControlBlockPtr_t pcb = removeFirstPCBFromList(scheduler.ready);
    setStackPointer(pcb, currentProcessStackPointer);
    addProcessToScheduler(pcb);
}

void addProcessToScheduler(processControlBlockPtr_t pcb) {
    if(isReady(pcb)) {
        addPCBToList(scheduler.ready, pcb);
    }
	else if(isWaiting(pcb) || isBlocked(pcb)) {
        addPCBToList(scheduler.waiting, pcb);
    }
    else if(isTerminate(pcb)) {
        addPCBToList(scheduler.terminated, pcb);
    }
}

void terminateCurrentProcess(int returnValue) {
    processControlBlockPtr_t currentPCB = getCurrentPCB();
    processControlBlockPtr_t currentPCBFather = getFather(currentPCB);
    giveChildsToFather(currentPCB);
    if(isWaiting(currentPCBFather)) {
        wakeUp(getPid(getFather(currentPCB)));
    }
    setReturnValue(currentPCB, returnValue);
    freeStack(currentPCB);
    if(getForegroundPid() == getProcessID()) {
        setForeground(currentPCBFather);
    }
    setState(currentPCB, PROCESS_TERMINATE);
    _force_context_switch();

}

void sleepCurrent(int condition) {
    processControlBlockPtr_t currentPCB = getCurrentPCB();
    setState(currentPCB, condition);
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
    processControlBlockPtr_t father = getCurrentPCB();
    processControlBlockPtr_t son = PCBFromListByPID(getSons(father), pid);
    if(son != NULL) {
        while (!isTerminate(son)) {
            sleepCurrent(PROCESS_WAITING);
        }
    }
    PCBFromListByPID(scheduler.terminated, pid);
    freeMemory(son);
    return getReturnValue(son);
}

processControlBlockPtr_t getASonOfCurrentProcess() {
    processControlBlockPtr_t current = getCurrentPCB();
    processControlBlockListPtr_t currentSons = getSons(current);
    return consultFirstPCBFromList(currentSons);
}

processControlBlockPtr_t getCurrentPCB() {
    return consultFirstPCBFromList(scheduler.ready);
}

uint64_t getProcessID() {
    return getPid(getCurrentPCB());
}

void printAllProcess() {
    printWithColor("The id of the process that are ready are:", 41, 10);
    printListPid(scheduler.ready);
    printWithColor("The id of the process that are waiting are:", 43, 10);
    printListPid(scheduler.waiting);
    printWithColor("The id of the process that are terminated are:", 46, 10);
    printListPid(scheduler.terminated);
}
