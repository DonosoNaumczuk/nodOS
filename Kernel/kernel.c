#include <kernel.h>
#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <RTCReader.h>
#include <idtLoader.h>
#include <videoDriver.h>
#include <memoryAllocator.h>
#include <mutualExclusion.h>
#include <processControlBlock.h>
#include <scheduler.h>
#include <semaphore.h>
#include <messageQueue.h>
#include <interrupts.h>
#include <pipe.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;


typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase() {
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				/* The size of the stack itself, 32KiB */
		- sizeof(uint64_t)			/* Begin at the top of the stack */
	);
}

void * initializeKernelBinary() {
	char buffer[10];
	(cpuVendor(buffer));
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);
	initializeVideoDriver();
	initialPrint();
	_setBinaryTime();
	initializeMemoryAllocator(getStackBase());
	initMutualExclusion();
	initSemaphores(SCHEDULER_PROCESS_ID);
	initPipes(SCHEDULER_PROCESS_ID);
	initMessageQueue();
	initializeScheduler();
	load_idt();
	clear();
	return getStackBase();
}

void goToEntryPoint() {
	((EntryPoint)sampleCodeModuleAddress)();
}

void initialPrint() {
	printWithColor("Initializing nodOS Kernel...", 28, 49);
	newLine();
	printWithColor("Dammiano Donoso_Naumczuk Izaguirre Negro_Caino", 46, 22);
	newLine();
}

void init(int cant, void ** args) {
	while(1) {
		_hlt();
	}
}

void cleaner(int cant, void ** args) {
	while(1) {
		processControlBlockPtr_t son = getASonOfCurrentProcess();
		if(son == NULL) {
			return;
		}
		waitChild(getProcessIdOf(son));
	}
}

int main() {
	int foreground = 0;
	void * initargs[] = {&foreground, "init", NULL, NULL};
	void * cleanerargs[] = {&foreground, "cleaner", NULL, NULL};
	void * shellargs[] = {&foreground, "shell", NULL, NULL};
	processControlBlockPtr_t initPCB = createProcess(NULL, &init, 4, initargs);
	processControlBlockPtr_t cleanerPCB = createProcess(initPCB, &cleaner, 4, cleanerargs);
	processControlBlockPtr_t shellPCB = createProcess(cleanerPCB, sampleCodeModuleAddress, 4, shellargs);
	setForeground(shellPCB);
	startScheduler();
	return 0;
}
