static uint8_t ticksPassed = 0;
static Scheduler scheduler;

void initializeScheduler() {
    scheduler.ready = initializePCBList();
    scheduler.waiting = initializePCBList();
}

void * schedule(void * currentProcessStackPointer) {
	ticksPassed ++;
	if(ticksPassed == QUANTUM) {
		ProcessControlBlock *pcb = removeFirstPCBFromList(scheduler->ready);
		pcb->stackpointer = currentProcessStackPointer;
		addPCBToList(scheduler->ready, pcb);
		ticksPassed = 0;
	}
	return consultFirstPCBFromList(scheduler->ready)->stackpointer;
}

void * addProcessToScheduler(ProcessControlBlock_t *pcb) {
    if(isReady(pcb)) {
        addPCBToList(scheduler->ready, pcb);
    }
	else if (isWaiting(pcb)) {
        addPCBToList(scheduler->waiting, pcb);
    }
}

void
