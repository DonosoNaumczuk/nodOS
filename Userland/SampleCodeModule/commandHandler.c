#include <commandHandler.h>



void printArgs(int *args, int size);


int  commandInterpreter(unsigned char buffer[],	unsigned int size){
	unsigned int argumentsStart;
	int cmdID;
	cmdID = readCommand(buffer,&argumentsStart);
	unsigned char* arguments = buffer + argumentsStart;
	uint64_t processId;
	switch(cmdID){
		case TIME:
			processId = createProcess(&printTime, 1, &arguments);
			return	waitChild(processId);
		case EXIT:
		return	exit_(arguments);
		case QUADRATIC:
			processId = createProcess(&graphQuadratic, 1, &arguments);
			return	waitChild(processId);
		case LINEAR:
			processId = createProcess(&graphLinear, 1, &arguments);
			return	waitChild(processId);
//			return	graphLinear(arguments);
		case HELP:
			processId = createProcess(&printHelp, 1, &arguments);
			return waitChild(processId);
		case TEST:
		return	test(arguments);
	}
	return 1;
}

int readCommand(unsigned char buffer[],int * argumentsStart) {
	unsigned char cmd[MAX_CMD_LONG];
	int i = 0;

	while((i < MAX_CMD_LONG) && (buffer[i] != 0) && (buffer[i] != ' ')){
		cmd[i] = buffer[i];
		i++;
	}

	if(i >= MAX_CMD_LONG)	return INVALID;
	cmd[i] = 0;
	*argumentsStart = (unsigned int) i;

	if(strcmp("time",cmd) == 0)				return	TIME;
	if(strcmp("help",cmd) == 0)				return	HELP;
	if(strncmp("quadratic",cmd,14) == 0)	return	QUADRATIC;
	if(strncmp("linear",cmd,11) == 0)		return	LINEAR;
	if(strncmp("echo",cmd,4) == 0)			return	ECHO;
	if(strcmp("exit",cmd) == 0)				return	EXIT;
	if(strncmp("test",cmd,4) == 0)			return	TEST;
	return INVALID;
}

int printTime(int argumentQuantity, void **argumentVector) {
	if(*(unsigned char*)(*argumentVector) != 0)	return ARGS_ERROR;
	unsigned char timeDate[TIMEDATE_FMT_LONG];	//constant TIMEDATE_FMT_LONG in system.h
	getTimeDateString(timeDate);
	printf("%s\n",timeDate);
	return	VALID_CMD;
}

unsigned int getIntArguments(unsigned char buffer[],int args[],unsigned int total) {
	unsigned int i = 0;
	unsigned int argNum = 0;
	unsigned int negativeFlag = 0;
	while(argNum < total){
		negativeFlag = 0;
		while(buffer[i] == ' ')	i++;
		if(buffer[i] != 0)	args[argNum]	=	charToInt(buffer + i);
		else	return	ARGS_ERROR;
		while((buffer[i] != ' ') && (buffer[i] != 0)){
			if(buffer[i] == '-' && !negativeFlag)	negativeFlag = 1;
			else if(!isNumber(buffer[i]))	return	ARGS_ERROR;
			i++;
		}
		argNum++;
	}
	return VALID_CMD;
}

int graphQuadratic(int argumentQuantity, void** argumentVector) {
	int args[5];	// a,b,c,xScale,yScale
	unsigned char *buffer = (unsigned char*) (*argumentVector);
	if(getIntArguments(buffer,args,5) != VALID_CMD)	return	ARGS_ERROR;	//Cantidad de argumentos invalida.
	//printArgs(args, 5); evans
	if(args[3] <= 0 || args [4] <= 0) {
		printf("Scales must be greater than zero\n");
		return ERROR_CMD;
	}
	graphInit();
	graphWithScale(args[0],args[1],args[2],args[3],args[4]);
	return VALID_CMD;
}

int graphLinear(int argumentQuantity, void** argumentVector) {
	int args[4];
	unsigned char *buffer = (unsigned char*)(*argumentVector);
	if(getIntArguments(buffer,args,4) != VALID_CMD)	return	ARGS_ERROR;
	//printArgs(args, 4);
	if(args[2] <= 0 || args [3] <= 0) {
		printf("Scales must be greater than zero\n");
		return ERROR_CMD;
	}
	graphInit();
	graphWithScale(0,args[0],args[1],args[2],args[3]);
	return	VALID_CMD;
}

int test(unsigned char* buffer) {
	if(*buffer != 0)	buffer++;
	else return ARGS_ERROR;
	int cmpRes = 0;
	if((cmpRes = strcmp("zerodiv",buffer)) == 0)		divide0();
	else if((cmpRes = strcmp("overflow",buffer)) == 0)	overflow();
	else if ((cmpRes = strcmp("opcode",buffer)) == 0)	invalidop();
	return	(cmpRes == 0?	VALID_CMD:ARGS_ERROR);
}

int printHelp(int argumentQuantity, void **argumentVector) {
	if(*(unsigned char *)(*argumentVector) != 0)	return	ARGS_ERROR;
	printf("Commands:\n");
	printf("          * time : print the time provided by the Real Time Clock (RTC)\n");
	printf("          * quadratic a b c xScale yScale : print a cuadratic fuction [ax^2 + bx + c]\n");
	printf("          * linear a b xScale yScale : print a linear function [ax + b]\n");
	printf("          * exit : exit \n");
	printf("          * test zerodiv/opcode/overflow : execute a dedicate test for the selected exception\n");
	return VALID_CMD;
}

int exit_(unsigned char* arguments) {
	if(*arguments == 0)	return	-1;
	else 	return	ARGS_ERROR;
}

void printArgs(int *args, int size) {//evans
	for (int  i = 0; i < size; i++) {
		printf("args[%d] = %d\n", i, args[i]);
	}
}