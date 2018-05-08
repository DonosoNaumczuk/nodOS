#ifndef	_command_interpreter_h_
#define	_command_interpreter_h_

#include  <lib.h>
#include  <string.h>
#include  <system.h>
#include  <quadraticGraficator.h>
#include  <shell.h>
#include  <exceptionSample.h>
#include  <syscall.h>

#define   MAX_CMD_LONG   15

#define   INVALID        -1
#define   EXIT            0
#define   TIME            1
#define   QUADRATIC       2
#define   LINEAR          3
#define   HELP            4
#define   ECHO            5
#define   TEST            6

int  commandInterpreter(unsigned char buffer[],	unsigned int size);
int printTime(int argumentQuantity, void **argumentVector);
int readCommand(unsigned char buffer[],int * argumentsStart);
unsigned int getIntArguments(unsigned char buffer[],int args[],unsigned int total);
int graphQuadratic(int argumentQuantity, void **argumentVector);
int graphLinear(int argumentQuantity, void **argumentVector);
int test(unsigned char* buffer);
int printHelp(int argumentQuantity, void **argumentVector);
int exit_(unsigned char* arguments);

#endif