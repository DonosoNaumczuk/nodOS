#include <taskControlBlock.h>

typedef struct {
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;
} stackFrame_t;

typedef struct taskControlBlock_t {
    uint64_t tid;
    uint8_t state;
    void * stackPointer;
    processControlBlockPtr_t pcb;
} taskControlBlock_t;

static long int tidCounter = 1;

taskControlBlockPtr_t createTask(processControlBlockPtr_t pcb, void *codeAddress, int argsQuantity, void ** processArgs) {
    taskControlBlockPtr_t newTCB = initializeTCB(pcb, codeAddress, argsQuantity, processArgs);
    addProcessToScheduler(newTCB);
    return newTCB;
}

taskControlBlockPtr_t initializeTCB(processControlBlockPtr_t pcb, void *codeAddress, int argsQuantity, void ** processArgs) {
	taskControlBlockPtr_t newTCB = allocateMemory(sizeof(taskControlBlock_t));
    newTCB->tid = tidCounter;
    tidCounter++;
    newTCB->state = TASK_READY;
    newTCB->pcb = pcb;
    newTCB->stackPointer = allocateMemory(SIZE_OF_STACK);
    newTCB->stackPointer = startStack(codeAddress, newTCB->stackPointer, argsQuantity, processArgs);
    return newTCB;
}

void setStackPointer(taskControlBlockPtr_t tcp, void * stackPointer) {
    tcp->stackPointer = stackPointer;
}

void * getStackPointer(taskControlBlockPtr_t tcp) {
    return tcp->stackPointer;
}

int isTerminate(taskControlBlockPtr_t tcb) {
    return tcb->state == TASK_TERMINATE;
}

int isWaiting(taskControlBlockPtr_t tcb) {
    return tcb->state == TASK_WAITING;
}

int isBlockedByTCB(taskControlBlockPtr_t tcb) {
    return tcb->state == TASK_BLOCKED;
}

int isReady(taskControlBlockPtr_t tcb) {
	return tcb->state == TASK_READY;
}

int isMainTask(taskControlBlockPtr_t tcb) {
    return tcb == getMainTask(tcb->pcb);
}

processControlBlockPtr_t getPCBOf(taskControlBlockPtr_t tcb) {
	if(tcb == NULL) {
		return FALSE;
	}
    return tcb->pcb;
}

uint64_t getTaskIdOf(taskControlBlockPtr_t tcb) {
    return tcb->tid;
}

void setState(taskControlBlockPtr_t tcb, int state) {
	tcb->state = state;
}

void terminateATask(taskControlBlockPtr_t tcb) {
    freeMemory(tcb->stackPointer);
	removeTCBFromPCB(tcb->pcb, tcb);
    tcb->state = TASK_TERMINATE;
	wakeUpAPCB(tcb->pcb);
}

void terminateATaskWrapper(taskControlBlockPtr_t tcb) {
	if(isMainTask(tcb)) {
		terminateAProcess(-1, tcb->pcb);
	}
	else {
		terminateATask(tcb);
	}
}

void startProcess(int argsQuantity, void ** processArgs, void * codeAddress) {
    int returnValue = ((int (*)(int, void**))(codeAddress))(argsQuantity, processArgs);
	terminateATaskWrapper(getCurrentTCB());
}

void * startStack(void * codeAddress, void * stackBaseAddress, int argsQuantity, void ** processArgs) {
	stackFrame_t * stackFrame = (stackFrame_t *)(stackBaseAddress + SIZE_OF_STACK -
                                 sizeof(stackFrame_t) - 1);

    stackFrame->gs 		=	0x000;
    stackFrame->fs 		=	0x000;
    stackFrame->r15		=	0x000;
    stackFrame->r14		=	0x000;
    stackFrame->r13		=	0x000;
    stackFrame->r12		=	0x000;
    stackFrame->r11		=	0x000;
    stackFrame->r10		=	0x000;
    stackFrame->r9 		=	0x000;
    stackFrame->r8 		=	0x000;
    stackFrame->rsi		=	processArgs;
    stackFrame->rdi		=	argsQuantity;
    stackFrame->rbp		=	stackBaseAddress;
    stackFrame->rdx		=	codeAddress;
    stackFrame->rcx		=	0x000;
    stackFrame->rbx		=	0x000;
    stackFrame->rax		=	0x000;

    /* Interupt Return Hook */
    stackFrame->rip 	=	(uint64_t) &startProcess;
    stackFrame->cs  	=	0x008;
    stackFrame->rflags 	= 	0x202;
    stackFrame->rsp 	=	(uint64_t) &(stackFrame->base);
    stackFrame->ss  	= 	0x000;
    stackFrame->base 	=	0x000;

    return (void *)stackFrame;
}

void printATask(taskControlBlockPtr_t tcb) {
	printPCB(tcb->pcb);
	printWithColor("    |    ", 9, 0x0F);
	if(tcb->state == TASK_READY) {
		printWithColor("Ready     ", 10, 0x0F);
	}
	else if(tcb->state == TASK_BLOCKED) {
		printWithColor("Block     ", 10, 0x0F);
	}
	else if(tcb->state == TASK_WAITING) {
		printWithColor("Waiting   ", 10, 0x0F);
	}
	else if(tcb->state == TASK_TERMINATE) {
		printWithColor("Terminated", 10, 0x0F);
	}
	printWithColor("|    ", 5, 0x0F);
	if(tcb->tid < 10) {
		printDecimal(0);
	}
	printDecimal(tcb->tid);
}
