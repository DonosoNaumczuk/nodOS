#include <lib.h>
#include <string.h>
#include <system.h>

#define	MAX_CMD_LONG	15	

#define	INVALID		-1
#define	EXIT		0
#define	TIME		1
#define	CUADRATIC	2
#define	LINEAR		3
#define	HELP		4
#define	ECHO		5	


int  commandInterpreter(char buffer[],	unsigned int size){
	unsigned int argumentsStart;
	int cmdID;

	cmdID = readCommand(buffer,&argumentsStart);

	switch(cmdID){
		case TIME:	printf("%s\n",getTimeDateString());	return 0;
		case EXIT:	return -1;
	}
	return 1;
}



int readCommand(char buffer[],int * argumentsStart){
	char cmd[MAX_CMD_LONG];
	int i = 0;

	while((i < MAX_CMD_LONG) && (buffer[i] != 0) && (buffer[i] != " ")){
		cmd[i] = buffer[i];
		i++;
	}

	if(i >= MAX_CMD_LONG)	return INVALID;
	cmd[i] = 0;
	*argumentsStart = (unsigned int) ++i;
	if(strcmp("time",cmd) == 0)	return	TIME;
	if(strcmp("help",cmd) == 0)	return	HELP;
	if(strcmp("graphCuadratic",cmd) == 0)	return	CUADRATIC;
	if(strcmp("graphLinear",cmd) == 0)	return	LINEAR;
	if(strcmp("echo",cmd) == 0)	return	ECHO;
	if(strcmp("exit",cmd) == 0)	return	EXIT;
	return INVALID;
}

