#include	<lib.h>
#include	<string.h>
#include	<commandHandler.h>
#include  <null.h>
#include  <pipe.h>


void clearLine(unsigned int lineLong) {
	int i;
	for(i = 0;i < lineLong; i++){
		printf("\b");
	}
}

void clearBuffer(unsigned char buffer[]) {
	int i;
	for(i = 0; i < MAX_CMD_LONG; i++) {
		buffer[i] = 0;
	}
}

void clearHist(unsigned char hist[HIST_LONG][MAX_CMD_LONG]) {
	int i,j;
	i = j = 0;
	for(j = 0; j < HIST_LONG; j++) {
		for(i = 0; i < MAX_CMD_LONG; i++) {
			hist[j][i] = 0;
		}
	}
}

int shell() {
	unsigned int exitFlag = 0;
	unsigned char buffer[MAX_CMD_LONG];
	unsigned char currentChar;
	unsigned int index = 0;
	unsigned char hist[HIST_LONG][MAX_CMD_LONG];
	unsigned int histCurrentIndex = 0;
	unsigned int histAccessIndex = 0;
	unsigned int histSize = 0;

	clearBuffer(buffer);
//	clearHist(hist);

	while(!exitFlag) {
		changeFontColor(LIGHT_GREY);
		printf(PROMPT);
		changeFontColor(WHITE);
		while((currentChar = getchar()) != '\n') {

			if(currentChar == '\b') {	/* BACKSPACE */

				if(index > 0) {
					buffer[--index] = 0;
					printf("\b");
				}
			}

			else if(currentChar == UP_ARROW || currentChar == DOWN_ARROW) {		/* ANY ARROW */

				if(currentChar == UP_ARROW) {

					if(histAccessIndex > 0)	{
						histAccessIndex--;
					}

					else if(histSize > 0) {
						histAccessIndex = histSize - 1;
					}
				}

				else {

					if(histAccessIndex < (histSize - 1)) {
						histAccessIndex++;
					}

					else	{
						histAccessIndex = 0;
					}
				}

				clearLine(index);
				clearBuffer(buffer);
				index = 0;
				while(hist[histAccessIndex][index] != 0) {
					index++;
				}

				strcpy(buffer,hist[histAccessIndex]);
				printf("%s",buffer);
			}

			else {	//ANY CHARACTER

				if(index < MAX_CMD_LONG) {
					buffer[index++] = currentChar;
					putChar(currentChar);
				}
			}
		}
			printf("\n");

		if(!lookForPipes(buffer, index, &exitFlag)) {
			validateCommand(buffer, index, &exitFlag, NULL, NULL);
		}
		if(exitFlag == 1) {
			break;
		}

		if(histCurrentIndex >= HIST_LONG) {
			histCurrentIndex = 0;
		}

		strcpy(hist[histCurrentIndex],buffer);
		histCurrentIndex++;

		if(histSize != HIST_LONG) {
			histSize++;
		}

		histAccessIndex = histCurrentIndex;
		clearBuffer(buffer);
		index = 0;
	}

	return 0;
}

int lookForPipes(unsigned char *buffer, unsigned int index, unsigned int *exitFlag) {
	unsigned int i;
	unsigned char command[MAX_VALID_CMD_LONG];
	int pipesFound = 0, beginning = 0;
	char *stdin = NULL;
	char *stdout = NULL;
	char pipes[MAX_PIPES_QUANTITY_ON_COMMAND][MAX_PIPE_LENGTH_NAME];
	for(i = 0; i < index; i ++) {
		if(buffer[i] == '|') {
			if(index < i + 2) {
				printf(INVALID_COMMAND_STR);
			}
			if(pipesFound > MAX_PIPES_QUANTITY_ON_COMMAND) {
				return pipesFound;
			}
			strncpy(command, buffer, beginning, i);
			generatePipeName(pipes, pipesFound);
			createPipe(pipes[pipesFound], 4096, 0);
			stdout = pipes[pipesFound];
			pipesFound++;
			callWithPipes(command, i - beginning, exitFlag, stdin, stdout);
			if(*exitFlag == 1) {
				return pipesFound;
			}
			stdin = stdout;
			beginning = i + 2;
		}
	}
	if(pipesFound > 0) {
		stdout = NULL;
		strncpy(command, buffer, beginning, i);
		validateCommand(command, i, exitFlag, stdin, stdout);
	}
	terminateAllPipes(pipes, pipesFound);
	return pipesFound;
}

void callWithPipes(unsigned char *command, unsigned int index, unsigned int *exitFlag,
				char *stdin, char *stdout) {

	if(index >= 1 && command[index - 1] != ' ') {
		printf(INVALID_COMMAND_STR);
	}
	index--;
	command[index] = 0;
	validateCommand(command, index, exitFlag, stdin, stdout);
}

void validateCommand(unsigned char *buffer, unsigned int index, unsigned int *exitFlag,
					char *stdin, char *stdout) {
	switch (commandInterpreter(buffer, index, stdin, stdout)) {
		case INVALID_CMD: 	printf(INVALID_COMMAND_STR);	break;
		case ERROR_CMD: 	printf(COMMAND_EXECUTED_WITH_ERROR_STR);	break;
		case ARGS_ERROR:	printf(COMMAND_NOT_EXECUTED_ARGS_STR);	break;
		case EXIT_CMD:		*exitFlag = 1;	break;
		case VALID_CMD:		break;
	}
}

void generatePipeName(char pipes[MAX_PIPES_QUANTITY_ON_COMMAND][MAX_PIPE_LENGTH_NAME],
					 int pipesFound) {
	unsigned char number[4]; 
	intToString(number, pipesFound);
	strcpy((unsigned char *) pipes[pipesFound], (unsigned char *)PIPE_PREFIX);
	strcpy((unsigned char *) (pipes[pipesFound] + strLength(PIPE_PREFIX)),
			number);
}

void terminateAllPipes(char pipes[MAX_PIPES_QUANTITY_ON_COMMAND][MAX_PIPE_LENGTH_NAME],
					 int pipesFound) {
	int i;
	for(i = 0; i < pipesFound; i++) {
		terminatePipe(pipes[i]);
	}
}