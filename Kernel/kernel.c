#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <RTCReader.h>
#include <idtLoader.h>
#include <videoDriver.h>
#include <memoryAllocator.h>
#include <mutualExclusion.h>
#include <processControlBlock.h>

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
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
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
	if(initializeMemoryAllocator(getStackBase()) == ERROR_STATE) {
		printWithColor("Error initializating memory allocator\n", 38, 49);
		/* evans: We must finish all the execution here... */
	} //evans: check if this is the
   	  //base address and do the #define
	initMutualExclusion();
	initializeScheduler();
	load_idt();
	clear();
	return getStackBase();
}

void goToEntryPoint() {
		((EntryPoint)sampleCodeModuleAddress)();
}

void initialPrint() {
	printWithColor("Welcome to nodOS", 16, 49);
	newLine();
	printWithColor("Dammiano Donoso_Naumczuk Izaguirre Negro_Caino", 46, 22);
	newLine();
	newLine();
}

void init(int cant, void ** args) {
	while(1) {
		_hlt(); //evans check if halt?
	}
}

void cleaner(int cant, void ** args) {
	while(1) {
		processControlBlockPtr_t son = getASonOfCurrentProcess();
		if(son == NULL) {
			return;
		}
		waitChild(getPid(son));
	}
}

int main() {
	processControlBlockPtr_t initPCB = createProcess(NULL, &init, 0, NULL);
	processControlBlockPtr_t cleanerPCB = createProcess(initPCB, &cleaner, 0, NULL);
	createProcess(cleanerPCB, sampleCodeModuleAddress, 0, NULL);
	startScheduler();
	return 0;
}
