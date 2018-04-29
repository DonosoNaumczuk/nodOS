#define PROCESS_READY 0
#define PROCESS_TERMINATE 1
#define PROCESS_NEW 2
#define PROCESS_WAITING 3

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

typedef struct ProcessControlBlock_t {
    uint64_t pid;
    uint8_t state;
    ProcessControlBlock_t *parent;
    ProcessControlBlock_tList *childs;
    void *stackPointer;
} ProcessControlBlock_t;

ProcessControlBlock_t *initializePCB(ProcessControlBlock_t *parent, void *codeAddress);
int isTerminateAndTheSameProcess(ProcessControlBlock_t *pcb, long int pid);
int isTerminate(ProcessControlBlock_t *pcb);
int isWaiting(ProcessControlBlock_t *pcb);
int isReady(ProcessControlBlock_t *pcb);
void startProcess(int argsQuantity, void ** processArgs, void * codeAddress);
void * startStack(void * codeAddress, void * stackBaseAddress, int argsQuantity,
                 void ** processArgs);
