#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <RTCReader.h>
#include <idtLoader.h>
#include <videoDriver.h>

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
	// print("  text: 0x");
	// ncPrintHex((uint64_t)&text);
	// ncNewline();
	// print("  rodata: 0x");
	// ncPrintHex((uint64_t)&rodata);
	// ncNewline();
	// print("  data: 0x");
	// ncPrintHex((uint64_t)&data);
	// ncNewline();
	// print("  bss: 0x");
	// ncPrintHex((uint64_t)&bss);
	// ncNewline();
	initializeVideoDriver();
	initialPrint();
	load_idt();
	_setBinaryTime();
	goToEntryPoint();
	clear();
	return getStackBase();
}

void goToEntryPoint() {
		((EntryPoint)sampleCodeModuleAddress)();
}

void initialPrint() {
	printWithColor("x64-ARQ_Kernel", 14, 49);
	newLine();
	printWithColor("Dammiano Donoso_Naumczuk Negro_Caino", 36, 22);
	newLine();
	newLine();
}

int main() {
	return 0;
}
