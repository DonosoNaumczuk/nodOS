#include <scheduler.h>

typedef struct scheduler_t {
   listObject_t ready;
   listObject_t waiting;
   listObject_t terminated;
} scheduler_t;

void _force_context_switch(void);
void _context_switch(void);
static int TCBComparatorByPID(void * pid, void * tcb);
static int TCBComparatorByTID(void * tid, void * tcb);

static uint8_t ticksPassed = 0;
static scheduler_t scheduler;
static int isFirst = TRUE;

void initializeScheduler() {
    scheduler.ready = newList();
    scheduler.waiting = newList();
    scheduler.terminated = newList();
    createMutualExclusion(SCHEDULER_MUTEX_ID, SCHEDULER_PROCESS_ID);
    lockIfUnlocked(SCHEDULER_MUTEX_ID, SCHEDULER_PROCESS_ID);
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
            nextTask(currentProcessStackPointer);
        }
        aux = getStackPointer(getCurrentTCB());
        unlock(SCHEDULER_MUTEX_ID, SCHEDULER_PROCESS_ID);
    }

	return aux;
}

void nextTask(void * currentProcessStackPointer) {
    taskControlBlockPtr_t tcb;
    getFirstElement(scheduler.ready, &tcb);
    removeAndFreeFirst(scheduler.ready);
    setStackPointer(tcb, currentProcessStackPointer);
    addProcessToScheduler(tcb);
}

void addProcessToScheduler(taskControlBlockPtr_t tcb) {
    if(isReady(tcb)) {
        addElement(scheduler.ready, &tcb, sizeof(tcb));
    }
	else if(isWaiting(tcb) || isBlockedByTCB(tcb)) {
        addElement(scheduler.waiting, &tcb, sizeof(tcb));
    }
    else if(isTerminate(tcb)) {
        if(isMainTask(tcb)) {
            addElement(scheduler.terminated, &tcb, sizeof(tcb));
        }
        else {
            freeMemory(tcb);
        }
    }
}

void terminateCurrentProcess(int returnValue) {
    terminateAProcess(returnValue, getCurrentPCB());
}

void terminateAProcessByPid(uint64_t pid) {
    if(pid != 1 && pid != 2) {
        processControlBlockPtr_t pcb = getPCBByPid(pid);
        if(pcb != NULL) {
            terminateAProcess(-1, pcb);
        }
    }
}

void terminateATaskByTid(uint64_t tid) {
    if(tid != 1 && tid != 2) {
        taskControlBlockPtr_t tcb = getTCBByTid(tid);
        if(tcb != NULL) {
            terminateATask(tcb);
        }
    }
}

processControlBlockPtr_t getPCBByPid(uint64_t pid) {
    taskControlBlockPtr_t tcb;
    if(getFirstElementByCriteria(scheduler.ready, &TCBComparatorByPID, &pid, &tcb) == ELEMENT_DOESNT_EXIST) {
        if(getFirstElementByCriteria(scheduler.waiting, &TCBComparatorByPID, &pid, &tcb) == ELEMENT_DOESNT_EXIST) {
            return NULL;
        }
    }
    return getPCBOf(tcb);
}

static int TCBComparatorByPID(void * pid, void * tcb) {
    taskControlBlockPtr_t tcbPointer = tcb;
    return getProcessIdOf(getPCBOf(*((taskControlBlockPtr_t *)tcb))) != *((uint64_t *)pid);
}

taskControlBlockPtr_t getTCBByTid(uint64_t tid) {
    taskControlBlockPtr_t tcb;
    if(getFirstElementByCriteria(scheduler.ready, &TCBComparatorByTID, &tid, &tcb) == ELEMENT_DOESNT_EXIST) {
        if(getFirstElementByCriteria(scheduler.waiting, &TCBComparatorByTID, &tid, &tcb) == ELEMENT_DOESNT_EXIST) {
            return NULL;
        }
    }
    return tcb;
}

static int TCBComparatorByTID(void * tid, void * tcb) {
    return getTaskIdOf(*((taskControlBlockPtr_t *)tcb)) != *((uint64_t *)tid);
}

int isBlocked(uint64_t tid) {
    taskControlBlockPtr_t tcb = getTCBByTid(tid);
    if(tcb == NULL) {
        return 0;
    }
    return isBlockedByTCB(tcb);
}

void sleepCurrent(int condition) {
    taskControlBlockPtr_t currentTCB = getCurrentTCB();
    setState(currentTCB, condition);
    _force_context_switch();
}

void wakeUp(uint64_t tid) {
    taskControlBlockPtr_t tcb;
    if(getFirstElementByCriteria(scheduler.waiting, &TCBComparatorByTID, &tid, &tcb) != ELEMENT_DOESNT_EXIST) {
        removeAndFreeFirstElementByCriteria(scheduler.waiting, &TCBComparatorByTID, &tid);
        setState(tcb, PROCESS_READY);
        addProcessToScheduler(tcb);
    }
}

int waitChild(uint64_t pid) {
    processControlBlockPtr_t father = getCurrentPCB();
    processControlBlockPtr_t son = PCBFromListByPID(getSons(father), pid);

    if(son != NULL) {
        while (!isProcessTerminate(son)) {
            sleepCurrent(PROCESS_WAITING);
        }
    }

    removeAndFreeFirstElementByCriteria(scheduler.terminated, &TCBComparatorByPID, &pid);

    int aux = getReturnValue(son);
    freeMemory(son);
    return aux;
}

processControlBlockPtr_t getASonOfCurrentProcess() {
    processControlBlockPtr_t current = getCurrentPCB();
    processControlBlockListPtr_t currentSons = getSons(current);
    return consultFirstPCBFromList(currentSons);
}

processControlBlockPtr_t getCurrentPCB() {
    return getPCBOf(getCurrentTCB());
}

taskControlBlockPtr_t getCurrentTCB() {
    taskControlBlockPtr_t tcb;
    getFirstElement(scheduler.ready, &tcb);
    return tcb;
}

uint64_t getProcessId() {
    return getProcessIdOf(getCurrentPCB());
}

uint64_t getTaskId() {
    return getTaskIdOf(getCurrentTCB());
}

int isCurrentForeground() {
    return 1;//evans
    //return isForeground(getCurrentPCB());
}

void printAllProcess() {
    /*printWithColor("N A M E             |   I D    |     M O D E      |  S T A T E\n", 63, 0x0F);
    printWithColor("--------------------------------------------------------------\n", 63, 0x0F);
    printList(scheduler.ready);
    printList(scheduler.waiting);
    printList(scheduler.terminated);*/
}
