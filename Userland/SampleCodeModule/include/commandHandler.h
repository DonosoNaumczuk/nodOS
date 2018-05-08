#ifndef	_command_interpreter_h_
#define	_command_interpreter_h_

int  commandInterpreter(unsigned char buffer[],	unsigned int size);
int printTime(int argumentQuantity, void **argumentVector);
int readCommand(unsigned char buffer[],int * argumentsStart);
unsigned int getIntArguments(unsigned char buffer[],int args[],unsigned int total);
int graphCuadratic(int argumentQuantity, void **argumentVector);
int graphLinear(unsigned char* buffer);
int test(unsigned char* buffer);
int printHelp(int argumentQuantity, void **argumentVector);
int exit_(unsigned char* arguments);

#endif