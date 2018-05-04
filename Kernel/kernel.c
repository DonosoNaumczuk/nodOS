#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <RTCReader.h>
#include <idtLoader.h>
#include <videoDriver.h>
#include <memoryAllocator.h>

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

/*evans test process 1*/
void test1(int cant, void ** args) {
	while(1) {
		printWithColor("Welcome to nodOS", 16, 49);
	}
}

/*evans test process 2*/
void test2(int cant, void ** args) {
	while(1) {
		printWithColor("Dammiano Donoso_Naumczuk Izaguirre Negro_Caino", 46, 22);
	}
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
	load_idt();
	if(initializeMemoryAllocator(getStackBase()) == ERROR_STATE) {
		printWithColor("Error initializating memory allocator\n", 38, 49);
		/* evans: We must finish all the execution here... */
	} //evans: check if this is the
   	  //base address and do the #define
	_setBinaryTime();
	/*evans beging scheduler test*/
	createProcess(NULL, &test1, 0, NULL);
	createProcess(NULL, &test2, 0, NULL);
	/*evans end of scheduler test*/
	startScheduler();
	while(1){ //evans need for test
		//rintWithColor("Fuck\n", 4, 49);
	}
	goToEntryPoint();
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

int main() {
	return 0;
}
