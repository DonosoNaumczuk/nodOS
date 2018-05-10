#ifndef	_command_interpreter_h_
#define	_command_interpreter_h_

#include  <lib.h>
#include  <string.h>
#include  <system.h>
#include  <quadraticGraficator.h>
#include  <shell.h>
#include  <exceptionSample.h>
#include  <syscall.h>
#include  <colors.h>
#include  <semaphore.h>
#include  <semaphoreTest.h>
#include  <culosucio.h>
#include  <prodcons.h>

#define   MAX_CMD_LONG   18

#define   INVALID           -1
#define   EXIT               0
#define   TIME               1
#define   QUADRATIC          2
#define   LINEAR             3
#define   HELP               4
#define   ECHO               5
#define   TEST               6
#define   CLEAN_SCREEN       7
#define   SEMAPHORE		     8
#define   PROCESS_LIST	     9
#define   CULO_SUCIO	     10
#define	  PRODUCTOR_CONSUMER 11
#define	  KILL				 12

#define   FOREGROUND	  1
#define	  BACKGROUND	  0

int  commandInterpreter(unsigned char buffer[],	unsigned int size);
int printTime(int argumentQuantity, void **argumentVector);
int readCommand(unsigned char buffer[],int * argumentsStart);
unsigned int getIntArguments(unsigned char buffer[],int args[],unsigned int total);
int graphQuadratic(int argumentQuantity, void **argumentVector);
int graphLinear(int argumentQuantity, void **argumentVector);
int test(int argumentQuantity, void** argumentVector);
int printHelp(int argumentQuantity, void **argumentVector);
int exit_(unsigned char* arguments);
void setArguments(void ** argVector, unsigned char *arguments,
 						uint64_t *processType,	char *processName);
int isBackground(char *arguments);
int getStartOfBackgroundParameter(char *arguments);
void printExitMessage();
int ps(int argumentQuantity, void **argumentVector);
int culoSucio(int argumentQuantity, void ** argumentVector);
int kill(int argumentQuantity, void ** argumentVector);

#endif
