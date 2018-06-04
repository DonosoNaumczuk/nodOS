#include <processControlBlock.h>

typedef struct processControlBlock_t {
    uint64_t pid;
    struct processControlBlock_t *parent;
    processControlBlockListPtr_t childs;
	char *name;
	int returnValue;
	int foreground;
	taskControlBlockPtr_t mainTask;
    char *readSource;
    char *writeSource;
	listObject_t othertasks;
    listObject_t heap;
} processControlBlock_t;

static int tcbComparator(void * tcb1, void * tcb2);
static int TCBComparatorByTID(void * tid, void * tcb);
static int addressComparator(void * address1, void * address2);

static long int pidCounter = 1;

processControlBlockPtr_t createProcess(processControlBlockPtr_t parent, void *codeAddress,
                                        int argsQuantity, void ** processArgs) {
	processControlBlock_t *newPCB = allocateMemory(sizeof(processControlBlock_t));
	newPCB->pid = pidCounter;
	pidCounter++;

	newPCB->parent = parent;
	newPCB->childs = initializePCBList();
	newPCB->name = (char *)(processArgs[1]);
     newPCB->readSource = (char *)(processArgs[2]);
     newPCB->writeSource = (char *)(processArgs[3]);


	newPCB->mainTask = createTask(newPCB, codeAddress, argsQuantity-4, processArgs+4);
	newPCB->othertasks = newList();
    newPCB->heap = newList();

	if(parent != NULL) {
		addPCBToList(parent->childs, newPCB);
	}

	if(*((uint64_t *)(*processArgs)) != FALSE && isForeground(parent)) {
		newPCB->foreground = TRUE;
	}
	else {
		newPCB->foreground = FALSE;
	}

	return newPCB;
}

uint64_t addTaskToProcess(processControlBlockPtr_t pcb, void *codeAddress, int argsQuantity, void ** processArgs) {
    taskControlBlockPtr_t newTCB = createTask(pcb, codeAddress, argsQuantity, processArgs);
    addElement(pcb->othertasks, &newTCB, sizeof(newTCB));
    return getTaskId(newTCB);
}

void * addMemoryToHeap(processControlBlockPtr_t pcb, uint64_t size) {
    void * address = allocateMemory(size);
    addElement(pcb->heap, &address, sizeof(address));
    return address;
}

char *getWriteSource(processControlBlockPtr_t pcb) {
    return pcb->writeSource;
}

char *getReadSource(processControlBlockPtr_t pcb) {
    return pcb->readSource;
}

void setReadSource(processControlBlockPtr_t pcb, char *readSource) {
     pcb->readSource = readSource;
}

void setWriteSource(processControlBlockPtr_t pcb, char *writeSource) {
     pcb->writeSource = writeSource;
}

int freeMemoryFromHeap(processControlBlockPtr_t pcb, void * address) {
    removeAndFreeFirstElementByCriteria(pcb->heap, &addressComparator, &address);
    return freeMemory(address);
}

static int addressComparator(void * address1, void * address2) {
    return *((void**) address1) != *((void**) address1);
}

void setForeground(processControlBlockPtr_t pcb) {
	pcb->foreground = TRUE;
}

int isForeground(processControlBlockPtr_t pcb) {
    if(pcb == NULL) {
        return FALSE;
    }
	return pcb->foreground;
}

uint64_t getProcessIdOf(processControlBlockPtr_t pcb) {
	return pcb->pid;
}

processControlBlockListPtr_t getSons(processControlBlockPtr_t pcb) {
	return pcb->childs;
}

taskControlBlockPtr_t getMainTask(processControlBlockPtr_t pcb) {
	return pcb->mainTask;
}

processControlBlockPtr_t getFather(processControlBlockPtr_t pcb) {
	return pcb->parent;
}

int isThisPid(processControlBlockPtr_t pcb, long int pid) {
    return pcb->pid == pid;
}

void giveChildsToFather(processControlBlockPtr_t pcb) {
	pcb->parent->childs = concatenatePCBList(pcb->parent->childs, pcb->childs);
}

void setReturnValue(processControlBlockPtr_t pcb, int returnValue) {
	pcb->returnValue = returnValue;
}

int getReturnValue(processControlBlockPtr_t son) {
	return son->returnValue;
}

void terminateAProcess(int returnValue, processControlBlockPtr_t pcb) {
    processControlBlockPtr_t currentPCBFather = getFather(pcb);
    processControlBlockPtr_t PCBCleaner = getPCBByPid(2);
    PCBCleaner->parent->childs = concatenatePCBList(PCBCleaner->childs, pcb->childs);

    wakeUpAPCB(currentPCBFather);
    setReturnValue(pcb, returnValue);

	terminateATask(pcb->mainTask);
	for(int i = 0; i < size(pcb->othertasks); i++) {
		taskControlBlockPtr_t aux;
		getElementOnIndex(pcb->othertasks, &aux, i);
	    terminateATask(aux);
	}

    _force_context_switch();
}

void wakeUpAPCB(processControlBlockPtr_t pcb) {
    if(isWaiting(pcb->mainTask)) {
        wakeUp(getTaskIdOf(pcb->mainTask));
    }
    for(int i = 0; i < size(pcb->othertasks); i++) {
        taskControlBlockPtr_t aux;
        getElementOnIndex(pcb->othertasks, &aux, i);
        if(isWaiting(aux)) {
            wakeUp(getTaskIdOf(aux));
        }
    }
}

void removeTCBFromPCB(processControlBlockPtr_t pcb, taskControlBlockPtr_t tcb) {
    removeAndFreeFirstElementByCriteria(pcb->othertasks, &tcbComparator, &tcb);
}

static int tcbComparator(void * tcb1, void * tcb2) {
    return getTaskIdOf(*((taskControlBlockPtr_t *)tcb1)) == getTaskIdOf(*((taskControlBlockPtr_t *)tcb2));
}

int isProcessTerminate(processControlBlockPtr_t pcb) {
    return isTerminate(pcb->mainTask);
}

taskControlBlockPtr_t getTaskByTid(processControlBlockPtr_t pcb, uint64_t tid) {
    taskControlBlockPtr_t tcb;
    getFirstElementByCriteria(pcb->othertasks, &TCBComparatorByTID, &tid, &tcb);
    return tcb;
}

static int TCBComparatorByTID(void * tid, void * tcb) {
    return getTaskIdOf(*((taskControlBlockPtr_t *)tcb)) != *((uint64_t *)tid);
}

void printPCB(processControlBlockPtr_t pcb) {
/*
     if(pcb != NULL) {
		char * aux = pcb->name;
		int i = 0;
		while(*aux != 0 && i < 20) {
			printWithColor(aux, 1, 0x0F);
			aux++;
			i++;
		}
		while (i < 20) {
			printWithColor(" ", 1, 0x0F);
			i++;
		}
		printWithColor("|    ", 5, 0x0F);
		if(pcb->pid < 10) {
			printDecimal(0);
		}
		printDecimal(pcb->pid);
		printWithColor("    |    ", 9, 0x0F);
		if(pcb->foreground) {
			printWithColor("Foreground", 10, 0x0F);
		}
		else {
			printWithColor("Background", 10, 0x0F);
		}
		printWithColor("    |    ", 9, 0x0F);
		if(pcb->state == PROCESS_READY) {
			printWithColor("Ready     ", 10, 0x0F);
		}
		else if(pcb->state == PROCESS_BLOCKED) {
			printWithColor("Block     ", 10, 0x0F);
		}
		else if(pcb->state == PROCESS_WAITING) {
			printWithColor("Waiting   ", 10, 0x0F);
		}
		else if(pcb->state == PROCESS_TERMINATE) {
			printWithColor("Terminated", 10, 0x0F);
		}
		newLine();
	}*/
}
