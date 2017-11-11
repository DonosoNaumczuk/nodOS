#include	<lib.h>
#include	<string.h>
#include	<system.h>
#include	<cuadraticGraficator.h>
#include	<shell.h>

#define	MAX_CMD_LONG	15	

#define	INVALID		-1
#define	EXIT		0
#define	TIME		1
#define	CUADRATIC	2
#define	LINEAR		3
#define	HELP		4
#define	ECHO		5	


int  commandInterpreter(unsigned char buffer[],	unsigned int size){
	unsigned int argumentsStart;
	int cmdID;
	cmdID = readCommand(buffer,&argumentsStart);

	switch(cmdID){
		case TIME:	printTime();	return 0;
		case EXIT:	return -1;
		case CUADRATIC:	return	graphCuadratic(buffer + argumentsStart);
		case LINEAR:	return	graphLinear(buffer + argumentsStart);
		case HELP:	return	printHelp();
	}
	return 1;
}

int readCommand(unsigned char buffer[],int * argumentsStart){
	unsigned char cmd[MAX_CMD_LONG];
	int i = 0;

	while((i < MAX_CMD_LONG) && (buffer[i] != 0) && (buffer[i] != ' ')){	
		cmd[i] = buffer[i];
		i++;
	}

	if(i >= MAX_CMD_LONG)	return INVALID;
	cmd[i] = 0;
	*argumentsStart = (unsigned int) i;

	if(strcmp("time",cmd) == 0)	return	TIME;
	if(strcmp("help",cmd) == 0)	return	HELP;
	if(strncmp("cuadratic",cmd,14) == 0)	return	CUADRATIC;
	if(strncmp("linear",cmd,11) == 0)	return	LINEAR;
	if(strncmp("echo",cmd,4) == 0)	return	ECHO;
	if(strcmp("exit",cmd) == 0)	return	EXIT;
	return INVALID;
}

void printTime(){
	unsigned char timeDate[TIMEDATE_FMT_LONG];	//constant TIMEDATE_FMT_LONG in system.h
	getTimeDateString(timeDate);
	printf("%s\n",timeDate);
	return;
}
unsigned int getIntArguments(unsigned char buffer[],int args[],unsigned int total){
	unsigned int i = 0;
	unsigned int argNum = 0;

	while(argNum < total){
		if(buffer[i] == ' ')	args[argNum]	=	charToInt(++i + buffer);
		else	return	ARGS_ERROR;
		while((buffer[i] != ' ') && (buffer[i] != 0))	i++;
		argNum++;
	}

	return VALID_CMD;
}

int graphCuadratic(unsigned char* buffer){
	int args[5];	// a,b,c,xScale,yScale	
	if(getIntArguments(buffer,args,5) != VALID_CMD)	return	ARGS_ERROR;	//Cantidad de argumentos invalida.
	graphInit();
	graphWithScale(args[0],args[1],args[2],args[3],args[4]);
	return VALID_CMD;
}

int graphLinear(unsigned char* buffer){
	int args[3];
	if(getIntArguments(buffer,args,3) != VALID_CMD)	return	ARGS_ERROR;
	graphInit();
	graphWithScale(0,args[0],args[1],args[2],args[3]);
	return	VALID_CMD;
}

int printHelp(){
	printf("Commands:\n");
	printf("          * time : print the time provided by the Real Time Clock (RTC)\n");
	printf("          * cuadratic a b c xScale yScale : print a cuadratic fuction [ax^2 + bx + c]\n");
	printf("          * linear a b xScale yScale : print a linear function[ax + b]\n");
	printf("          * exit : exit \n");
	return VALID_CMD;
}

