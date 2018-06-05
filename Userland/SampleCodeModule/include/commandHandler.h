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
#include  <prodcons.h>
#include  <pipeTest.h>

#define   INVALID            -1
#define   EXIT                0
#define   TIME                1
#define   QUADRATIC           2
#define   LINEAR              3
#define   HELP                4
#define   ECHO                5
#define   TEST                6
#define   CLEAN_SCREEN        7
#define   SEMAPHORE		     8
#define   PROCESS_LIST	     9
#define	PRODUCTOR_CONSUMER 10
#define	TERMINATE_PROCESS  11
#define   PIPE               12
#define   GREP               13
#define   TERMINATE_THREAD   14
#define   THREAD_TEST        15

#define   FOREGROUND	  1
#define	BACKGROUND	  0

int commandInterpreter(unsigned char buffer[], unsigned int length, char *stdin,
                         char *stdout);
int printTime(int argumentQuantity, void ** argumentVector);
int readCommand(unsigned char buffer[], int *argumentsStart, unsigned int length);
unsigned int getIntArguments(unsigned char buffer[], int args[], unsigned int total);
int graphQuadratic(int argumentQuantity, void ** argumentVector);
int graphLinear(int argumentQuantity, void ** argumentVector);
int test(int argumentQuantity, void ** argumentVector);
int printHelp(int argumentQuantity, void **argumentVector);
int exit_(unsigned char *arguments);
void setArguments(void ** argVector, unsigned char *arguments, uint64_t *processType,
                    char *processName, char *stdin, char *stdout);
int isBackground(char *arguments);
int getStartOfBackgroundParameter(char *arguments);
void printExitMessage();
int ps(int argumentQuantity, void ** argumentVector);
int culoSucio(int argumentQuantity, void ** argumentVector);
int terminate(int argumentQuantity, void ** argumentVector);
uint64_t stringToPid(unsigned char *pidString);
int echo(int argumentQuantity, void ** argumentVector);
int grepChar(int argumentQuantity, void ** argumentVector);
int terminateThreadCommand(int argumentQuantity, void ** argumentVector);
int threadTest(int argumentQuantity, void ** argumentVector);
int loopThread(int argumentQuantity, void ** argumentVector);

#endif
