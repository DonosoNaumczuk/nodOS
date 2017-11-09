#include <lib.h>
#include <commandHandler.h>

#define	MAX_CMD_LONG	100
#define INVALID_CMD		1
#define	ERROR_CMD		2
#define	EXIT_CMD		-1
#define	VALID_CMD 		0


int shell(){
	unsigned int exitFlag = 0;
	char buffer[MAX_CMD_LONG];
	char currentChar;
	unsigned int index = 0;
	clearBuffer(buffer);

	while(!exitFlag){
		printf("~	");
		while((currentChar = getchar()) != '\n'){
			if(currentChar == '\b'){
				if(index>0){
					buffer[--index] = 0;
					printf("\b");
				}
			}else{
				if(index < MAX_CMD_LONG){
					buffer[++index]	= currentChar;
					printf("%s",buffer);
				}
			}
		}
		printf("\n");
		switch (commandInterpreter(buffer,index)){
			case INVALID_CMD: 	printf("--INVALID COMMAND--\n");	break;
			case ERROR_CMD: 	printf("--COMMAND EXECUTED WITH ERROR--\n");	break;
			case EXIT_CMD:		exitFlag = 1;	break;
			case VALID_CMD:		break;
		}
		clearBuffer(buffer);
		index = 0;
	}
}

void backspace(){
}

void clearLine(unsigned int lineLong){
	int i;
	for(i = 0;i < lineLong; i++) printf("\b");
}

void clearBuffer(char buffer[]){
	int i;
	for(i = 0; i < MAX_CMD_LONG; i++) buffer[i] = 0;
}