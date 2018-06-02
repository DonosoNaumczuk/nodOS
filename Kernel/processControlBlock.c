#include <processControlBlock.h>

typedef struct processControlBlock_t {
    uint64_t pid;
    struct processControlBlock_t *parent;
    processControlBlockListPtr_t childs;
	char *name;
	int returnValue;
	int foreground;
	taskControlBlockPtr_t mainTask;
	listObject_t othertasks;
    //list with heap evans
} processControlBlock_t;

static long int pidCounter = 1;

processControlBlockPtr_t createProcess(processControlBlockPtr_t parent, void *codeAddress, int argsQuantity, void ** processArgs) {
	processControlBlock_t *newPCB = allocateMemory(sizeof(processControlBlock_t));
	newPCB->pid = pidCounter;
	pidCounter++;
	newPCB->parent = parent;
	newPCB->childs = initializePCBList();
	newPCB->name = (char *)(processArgs[1]);

	newPCB->mainTask = createTask(newPCB, codeAddress, argsQuantity-2, processArgs+2);
	newPCB->othertasks = newList();

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

void setForeground(processControlBlockPtr_t pcb) {
	pcb->foreground = TRUE;
}

int isForeground(processControlBlockPtr_t pcb) {
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

	if(isWaiting(currentPCBFather->mainTask)) {
		wakeUp(getTaskIdOf(currentPCBFather->mainTask));
	}
	for(int i = 0; i < size(currentPCBFather->othertasks); i++) {
		taskControlBlockPtr_t aux;
		getElementOnIndex(currentPCBFather->othertasks, &aux, i);
	    if(isWaiting(aux)) {
	        wakeUp(getTaskIdOf(aux));
	    }
	}

    setReturnValue(pcb, returnValue);

	terminateATask(pcb->mainTask);
	for(int i = 0; i < size(pcb->othertasks); i++) {
		taskControlBlockPtr_t aux;
		getElementOnIndex(pcb->othertasks, &aux, i);
	    terminateATask(aux);
	}

    _force_context_switch();
}

/*void printPCB(processControlBlockPtr_t pcb) {

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
	}
}*/
