#ifndef	_command_interpreter_h_
#define	_command_interpreter_h_

int  commandInterpreter(unsigned char buffer[],	unsigned int size);
int printTime(int argc, void **argumentVector);
int readCommand(unsigned char buffer[],int * argumentsStart);
unsigned int getIntArguments(unsigned char buffer[],int args[],unsigned int total);
int graphCuadratic(unsigned char* buffer);
int graphLinear(unsigned char* buffer);
int test(unsigned char* buffer);
int printHelp(unsigned char* arguments);
int exit_(unsigned char* arguments);

#endif