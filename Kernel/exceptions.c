#include <videoDriver.h>

#define ZERO_EXCEPTION_ID 0
#define OVERFLOW_EXCEPTION_ID 1
#define INVALIDOPCODE_EXCEPTION_ID 2

static void zero_division();
static void over_flow();
static void invalid_operation_code();

void exceptionDispatcher(int exception) {
	if(exception == ZERO_EXCEPTION_ID)
		zero_division();
	if(exception == OVERFLOW_EXCEPTION_ID)
		over_flow();
	if(exception == INVALIDOPCODE_EXCEPTION_ID)
		invalid_operation_code();
}

static void zero_division() {
	clear();
	printRegisters();
	printWithColor("Error, divide 0 by 0",21,0x0F);
	haltCycle();
}

static void over_flow(){
	clear();
	printRegisters();
	printWithColor("Error, overflow",15,0x0F);
	haltCycle();
}

static void invalid_operation_code(){
	clear();
	printRegisters();
	printWithColor("Error, invalid operation code",29,0x0F);
	haltCycle();
}
