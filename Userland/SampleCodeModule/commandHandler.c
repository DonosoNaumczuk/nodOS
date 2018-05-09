#include <commandHandler.h>



void printArgs(int *args, int size);


int  commandInterpreter(unsigned char buffer[],	unsigned int size){
	unsigned int argumentsStart;
	int cmdID;
	cmdID = readCommand(buffer,&argumentsStart);
	unsigned char* arguments = buffer + argumentsStart;
	void ** argVector = allocateMemory(sizeof(void*) * 3);
	uint64_t processId;
	uint64_t processType;
	switch(cmdID) {
		case TIME:
			setArguments(argVector, arguments, &processType, "time");
			processId = createProcess(&printTime, 1, argVector);
			if(processType == FOREGROUND) {
				return	waitChild(processId);
			}
			else {
				return 0;
			}
		case EXIT:
		return	exit_(arguments);
		case QUADRATIC:
			setArguments(argVector, arguments, &processType, "quadratic");
			processId = createProcess(&graphQuadratic, 1, argVector);
			if(processType == FOREGROUND) {
				return	waitChild(processId);
			}
			else {
				return 0;
			}

		case LINEAR:
			setArguments(argVector, arguments, &processType, "linear");
			processId = createProcess(&graphLinear, 1, argVector);
			if(processType == FOREGROUND) {
				return	waitChild(processId);
			}
			else {
				return 0;
			}
		case HELP:
			setArguments(argVector, arguments, &processType, "help");
			processId = createProcess(&printHelp, 1, argVector);
			if(processType == FOREGROUND) {
				return	waitChild(processId);
			}
			else {
				return 0;
			}
			
		case TEST:
			setArguments(argVector, arguments, &processType, "test");
			processId = createProcess(&test, 1, argVector);
			if(processType == FOREGROUND) {
				return	waitChild(processId);
			}
			else {
				return 0;
			}
			

		case CLEAN_SCREEN:
				cleanScreen();
			return;
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
	if(strncmp("quadratic", cmd, 14) == 0)	return	QUADRATIC;
	if(strncmp("linear",cmd,11) == 0)		return	LINEAR;
	if(strncmp("echo",cmd,4) == 0)			return	ECHO;
	if(strcmp("exit",cmd) == 0)				return	EXIT;
	if(strncmp("test",cmd,4) == 0)			return	TEST;
	if(strncmp("clean", cmd, 5) == 0) 	return	CLEAN_SCREEN;
	return INVALID;
}

int printTime(int argumentQuantity, void **argumentVector) {
	if(*(unsigned char*)(*argumentVector) != 0)	return ARGS_ERROR;
	unsigned char timeDate[TIMEDATE_FMT_LONG];	//constant TIMEDATE_FMT_LONG in system.h
	getTimeDateString(timeDate);
	printf("%s\n",timeDate);
	return	VALID_CMD;
}

unsigned int getIntArguments(unsigned char buffer[], int args[], unsigned int total) {
	unsigned int i = 0;
	unsigned int argNum = 0;
	unsigned int negativeFlag = 0;
	while(argNum < total){
		negativeFlag = 0;
		while(buffer[i] == ' ') {
			i++;
		}
		if(buffer[i] != 0) {
			args[argNum]	=	charToInt(buffer + i);
		}
		else {
			return	ARGS_ERROR;
		}
		while((buffer[i] != ' ') && (buffer[i] != 0)) {
			if(buffer[i] == '-' && !negativeFlag)	negativeFlag = 1;
			else if(!isNumber(buffer[i]))	{
				return	ARGS_ERROR;
			}
			i++;
		}
		argNum++;
	}
	return VALID_CMD;
}

int graphQuadratic(int argumentQuantity, void** argumentVector) {
	int args[5];	// a,b,c,xScale,yScale
	unsigned char *buffer = (unsigned char *) (*argumentVector);
	if(getIntArguments(buffer, args, 5) != VALID_CMD) {
		return	ARGS_ERROR;	//Cantidad de argumentos invalida.
	}
	if(args[3] <= 0 || args [4] <= 0) {
		printf("Scales must be greater than zero\n");
		return ERROR_CMD;
	}
	graphInit();
	graphWithScale(args[0],args[1],args[2],args[3],args[4]);
	return VALID_CMD;
}

int graphLinear(int argumentQuantity, void** argumentVector) {
	int args[4];//a b xScale yScale
	unsigned char *buffer = (unsigned char*)(*argumentVector);
	if(getIntArguments(buffer,args,4) != VALID_CMD)	return	ARGS_ERROR;
	if(args[2] <= 0 || args [3] <= 0) {
		printf("Scales must be greater than zero\n");
		return ERROR_CMD;
	}
	graphInit();
	graphWithScale(0,args[0],args[1],args[2],args[3]);
	return	VALID_CMD;
}

int test(int argumentQuantity, void** argumentVector) {
	unsigned char *buffer = (unsigned char*)(*argumentVector);
	if(*buffer != 0)	buffer++;
	else return ARGS_ERROR;
	int cmpRes = 0;
	if((cmpRes = strcmp("zerodiv", buffer)) == 0)		divide0();
	else if ((cmpRes = strcmp("opcode", buffer)) == 0)	invalidop();
	return	(cmpRes == 0?	VALID_CMD:ARGS_ERROR);
}

int printHelp(int argumentQuantity, void **argumentVector) {
	if(*(unsigned char *)(*argumentVector) != 0)	return	ARGS_ERROR;
	printf("Commands:\n");
	printf("          * time : print the time provided by the Real Time Clock (RTC)\n");
	printf("          * quadratic a b c xScale yScale : print a cuadratic fuction [ax^2 + bx + c]\n");
	printf("          * linear a b xScale yScale : print a linear function [ax + b]\n");
	printf("          * exit : exit \n");
	printf("          * clean : clears the Screen \n");
	printf("          * test zerodiv/opcode : execute a dedicate test for the selected exception\n");
	return VALID_CMD;
}

int exit_(unsigned char* arguments) {
	if(*arguments == 0)	{
		changeFontColor(49);
		//printf("bye bye.\n");
		printExitMessage();
		return	-1;
	}
	else {
		return	ARGS_ERROR;
	}
}

void printExitMessage() {
printf("                                                                                                                              \n");
printf("   bbbbbbbb                                                        bbbbbbbb                                                   \n");
printf("   b::::::b                                                        b::::::b                                                   \n");
printf("   b::::::b                                                        b::::::b                                                   \n");
printf("   b::::::b                                                        b::::::b                                                   \n");
printf("    b:::::b                                                         b:::::b                                                   \n");
printf("    b:::::bbbbbbbbb yyyyyyy           yyyyyyy eeeeeeeeeeee          b:::::bbbbbbbbb yyyyyyy           yyyyyyy eeeeeeeeeeee    \n");
printf("    b::::::::::::::bby:::::y         y:::::yee::::::::::::ee        b::::::::::::::bby:::::y         y:::::yee::::::::::::ee  \n");
printf("    b::::::::::::::::by:::::y       y:::::ye::::::eeeee:::::ee      b::::::::::::::::by:::::y       y:::::ye::::::eeeee:::::ee\n");
printf("    b:::::bbbbb:::::::by:::::y     y:::::ye::::::e     e:::::e      b:::::bbbbb:::::::by:::::y     y:::::ye::::::e     e:::::e\n");
printf("    b:::::b    b::::::b y:::::y   y:::::y e:::::::eeeee::::::e      b:::::b    b::::::b y:::::y   y:::::y e:::::::eeeee::::::e\n");
printf("    b:::::b     b:::::b  y:::::y y:::::y  e:::::::::::::::::e       b:::::b     b:::::b  y:::::y y:::::y  e:::::::::::::::::e \n");
printf("    b:::::b     b:::::b   y:::::y:::::y   e::::::eeeeeeeeeee        b:::::b     b:::::b   y:::::y:::::y   e::::::eeeeeeeeeee  \n");
printf("    b:::::b     b:::::b    y:::::::::y    e:::::::e                 b:::::b     b:::::b    y:::::::::y    e:::::::e           \n");
printf("    b:::::bbbbbb::::::b     y:::::::y     e::::::::e                b:::::bbbbbb::::::b     y:::::::y     e::::::::e          \n");
printf("    b::::::::::::::::b       y:::::y       e::::::::eeeeeeee        b::::::::::::::::b       y:::::y       e::::::::eeeeeeee  \n");
printf("    b:::::::::::::::b       y:::::y         ee:::::::::::::e        b:::::::::::::::b       y:::::y         ee:::::::::::::e  \n");
printf("    bbbbbbbbbbbbbbbb       y:::::y            eeeeeeeeeeeeee        bbbbbbbbbbbbbbbb       y:::::y            eeeeeeeeeeeeee  \n");
printf("                          y:::::y                                                         y:::::y                             \n");
printf("                         y:::::y                                                         y:::::y                              \n");
printf("                        y:::::y                                                         y:::::y                               \n");
printf("                       y:::::y                                                         y:::::y                                \n");
printf("                      yyyyyyy                                                         yyyyyyy                                 \n");
printf("                                                                                                                              \n");
printf("                                                                                                                              \n");
}


void printArgs(int *args, int size) {//evans
	for (int  i = 0; i < size; i++) {
		printf("args[%d] = %d\n", i, args[i]);
	}
}

void setArguments(void ** argVector, unsigned char *arguments,
 						uint64_t *processType,	char *processName) {
	*processType = FOREGROUND;

	if(isBackground(arguments)) {
		*processType = BACKGROUND;
	}
	*argVector = processType;
	*(argVector + 1) = processName;
	*(argVector + 2) = arguments;
	return;
}

int isBackground(char * arguments) {
	int index = getStartOfBackgroundParameter(arguments);
	if(index == -1) {
		return 0;
	}
	return strncmp(arguments + index, "-b", strLength("-b")) == 0;
}

int getStartOfBackgroundParameter(char * arguments) {
	int i;
	for(i = 0; arguments[i] != 0; i++);
	if(i > 0 ) {
		i = i - 1;
		while(i > 0 && arguments[i] == ' ') {
			i--;
		}
		if(i >= (strLength("-b") - 1)) {
			i = i - (strLength("-b") - 1);
			return i;
		}

	}
	return -1;
}