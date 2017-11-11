#include	<lib.h>
#include	<string.h>
#include	<commandHandler.h>

#define	MAX_CMD_LONG	100
#define	HIST_LONG		100

#define UP_ARROW		133
#define DOWN_ARROW		134
#define LEFT_ARROW		135
#define RIGHT_ARROW		136

#define	EXIT_CMD	   -1
#define	VALID_CMD 		0
#define INVALID_CMD		1
#define	ERROR_CMD		2
#define	ARGS_ERROR		3

void clearLine(unsigned int lineLong){
	int i;
	for(i = 0;i < lineLong; i++) printf("\b");
}

void clearBuffer(unsigned char buffer[]){
	int i;
	for(i = 0; i < MAX_CMD_LONG; i++) buffer[i] = 0;
}

void clearHist(unsigned char hist[HIST_LONG][MAX_CMD_LONG]){
	int i,j;
	i = j = 0;
	for(j = 0; j < HIST_LONG; j++){
		for(i = 0; i<MAX_CMD_LONG; i++)	hist[j][i] = 0;
	}
}

int shell(){
	unsigned int exitFlag = 0;
	unsigned char buffer[MAX_CMD_LONG];
	unsigned char currentChar;
	unsigned int index = 0;
	unsigned char hist[HIST_LONG][MAX_CMD_LONG];
	unsigned int histCurrentIndex = 0;
	unsigned int histSize = 0;

	clearBuffer(buffer);
	clearHist(hist);

	while(!exitFlag){
		printf("~	");
		while((currentChar = getchar()) != '\n'){
			if(currentChar == '\b'){	//BACKSPACE
				if(index>0){
					buffer[--index] = 0;
					printf("\b");
				}
			}else if(currentChar == UP_ARROW || currentChar == DOWN_ARROW){		//ANY ARROW
				
				if(currentChar == UP_ARROW){
					if(histCurrentIndex > 0)	histCurrentIndex--;
					else if(histSize > 0)	histCurrentIndex = histSize - 1;
				}else{
					if(histCurrentIndex < (histSize - 1))	histCurrentIndex++;
					else	histCurrentIndex = 0;
				}
				clearLine(index);
				clearBuffer(buffer);
				index = 0;
				while(hist[histCurrentIndex][index] != 0)	index++;
				strcpy(buffer,hist[histCurrentIndex]);
				printf("%s",buffer);
			}else{	//ANY CHARACTER
				if(index < MAX_CMD_LONG){
					buffer[index++]	= currentChar;
					putChar(currentChar);
				}
			}
		}
		printf("\n");
		switch (commandInterpreter(buffer,index)){
			case INVALID_CMD: 	printf("-------------------------INVALID COMMAND---------------------------\n");	break;
			case ERROR_CMD: 	printf("--------------------COMMAND EXECUTED WITH ERROR--------------------\n");	break;
			case ARGS_ERROR:	printf("-------------------TOO FEW OR TOO MANY ARGUMENTS-------------------\n");	break;
			case EXIT_CMD:		exitFlag = 1;	break;
			case VALID_CMD:		break;
		}
		if(histCurrentIndex >= HIST_LONG)	histCurrentIndex = 0;
		strcpy(hist[histCurrentIndex],buffer);
		histCurrentIndex++;
		if(histSize != HIST_LONG)	histSize++;
		clearBuffer(buffer);
		index = 0;
	}
}
