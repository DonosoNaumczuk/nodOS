#include <processControlBlock.h>

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

typedef struct processControlBlock_t {
    uint64_t pid;
    uint8_t state;
    struct processControlBlock_t *parent;
    processControlBlockListPtr_t childs;
    void *stackPointer;
} processControlBlock_t;

static long int pidCounter = 1;

void createProcess(processControlBlockPtr_t parent, void *codeAddress, int argsQuantity, void ** processArgs) {
	processControlBlock_t *newPCB = initializePCB(parent, codeAddress, argsQuantity, processArgs);
	addProcessToScheduler(newPCB);
}

uint64_t getPid(processControlBlockPtr_t pcb) {
	return pcb->pid;
}

processControlBlockPtr_t initializePCB(processControlBlockPtr_t parent, void *codeAddress, int argsQuantity, void ** processArgs) {
    processControlBlock_t *newPCB = allocateMemory(sizeof(processControlBlock_t));
    newPCB->pid = pidCounter;
    pidCounter++;
    newPCB->parent = parent;
    newPCB->childs = initializePCBList();
    newPCB->stackPointer = allocateMemory(SIZE_OF_STACK);
    newPCB->state = PROCESS_READY;

    newPCB->stackPointer = startStack(codeAddress, newPCB->stackPointer, argsQuantity, processArgs);

    return newPCB;
}

void * getStackPointer(processControlBlockPtr_t pcb) {
    return pcb->stackPointer;
}

processControlBlockPtr_t getFather(processControlBlockPtr_t pcb) {
	return pcb->parent;
}

void setStackPointer(processControlBlockPtr_t pcb, void * stackPointer) {
    pcb->stackPointer = stackPointer;
}

int isThisPid(processControlBlockPtr_t pcb, long int pid) {
    return pcb->pid == pid;
}

int isTerminate(processControlBlockPtr_t pcb) {
    return pcb->state == PROCESS_TERMINATE;
}

int isWaiting(processControlBlockPtr_t pcb) {
    return pcb->state == PROCESS_WAITING;
}

int isReady(processControlBlockPtr_t pcb) {
	return pcb->state == PROCESS_READY;
}

void setState(processControlBlockPtr_t pcb, int state) {
	pcb->state = state;
}

void startProcess(int argsQuantity, void ** processArgs, void * codeAddress) {
    ((int (*)(int, void**))(codeAddress))(argsQuantity, processArgs);

	terminateCurrentProcess();
}

void giveChildsToFather(processControlBlockPtr_t pcb) {
	pcb->parent->childs = concatenatePCBList(pcb->parent->childs, pcb->childs);
}

void * startStack(void * codeAddress, void * stackBaseAddress, int argsQuantity,
                 void ** processArgs) {
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
