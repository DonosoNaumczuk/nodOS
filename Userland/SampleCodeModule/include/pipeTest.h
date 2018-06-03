#ifndef PIPE_TEST_H
#define PIPE_TEST_H

#define PIPE_NAME   "__pipeTest"
#define MESSAGE_LENGTH 100


int pipe(int argumentQuantity, void **argumentVector);
int readPipe(int argumentQuantity, void ** argumentVector);
int writePipe(int argumentQuantity, void ** argumentVector);

#endif