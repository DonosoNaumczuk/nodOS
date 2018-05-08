#ifndef _shell_h
#define _shell_h

#include <stdint.h>

#define	EXIT_CMD	   -1
#define	VALID_CMD 		0
#define INVALID_CMD		1
#define	ERROR_CMD		2
#define	ARGS_ERROR		3
#define   NULL (void * ) 0

int shell();
void receiveTest();
void sendTest();

#endif