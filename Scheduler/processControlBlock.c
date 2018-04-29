static long int pidCounter = 0;

ProcessControlBlock_t *initializePCB(ProcessControlBlock_t *parent, void *codeAddress) {
    ProcessControlBlock_t *newPCB = malloc(sizeof(ProcessControlBlock_t));
    newPCB->pid = pidCounter;
    pidCounter++;
    newPCB->parent = parent;
    newPCB->childs = initializePCBList();
    newPCB->stackPointer = malloc(4K);
    newPCB->state = PROCESS_READY;

    startStack(codeAddress, newPCB->stackPointer);
    addProcessToScheduler(newPCB);

    return newPCB;
}

int isTerminateAndTheSameProcess(ProcessControlBlock_t *pcb, long int pid) {
    return pcb->pid == pid && isTerminate(pcb);
}

int isTerminate(ProcessControlBlock_t *pcb) {
    return pcb->state == PROCESS_TERMINATE;
}

int isWaiting(ProcessControlBlock_t *pcb) {
    return pcb->state == PROCESS_WAITING;
}

int isReady(ProcessControlBlock_t *pcb) {
	return pcb->state == PROCESS_READY;
}

void startProcess(int argsQuantity, void ** processArgs, void * codeAddress) {
    ((int (*)(int, void**))(start))(cargs, pargs);

    //terminate process
}

void * startStack(void * codeAddress, void * stackBaseAddress, int argsQuantity,
                 void ** processArgs) {
	stackFrame_t * stackFrame = (stackFrame_t *)(stackBaseAddress -
                                 sizeof(stackFrame_t) - 1);

    stackFrame->gs 		=	0x001;
    stackFrame->fs 		=	0x002;
    stackFrame->r15		=	0x003;
    stackFrame->r14		=	0x004;
    stackFrame->r13		=	0x005;
    stackFrame->r12		=	0x006;
    stackFrame->r11		=	0x007;
    stackFrame->r10		=	0x008;
    stackFrame->r9 		=	0x009;
    stackFrame->r8 		=	0x00A;
    stackFrame->rsi		=	processArgs;
    stackFrame->rdi		=	argsQuantity;
    stackFrame->rbp		=	0x00D;
    stackFrame->rdx		=	codeAddress;
    stackFrame->rcx		=	0x00F;
    stackFrame->rbx		=	0x010;
    stackFrame->rax		=	0x011;

    /* Interupt Return Hook */
    stackFrame->rip 	=	(void*) &startProcess;
    stackFrame->cs  	=	0x008;
    stackFrame->eflags 	= 	0x202;
    stackFrame->rsp 	=	(uint64_t) &(stackFrame->base);
    stackFrame->ss  	= 	0x000;
    stackFrame->base 	=	0x000;

    return (void *)stackFrame;
}
