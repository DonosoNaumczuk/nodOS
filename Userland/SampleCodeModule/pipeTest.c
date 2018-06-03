#include <pipeTest.h>
#include <stdint.h>
#include <syscall.h>
#include <lib.h>
#include <string.h>



int pipe(int argumentQuantity, void **argumentVector) {
     uint64_t pid[2];
     void ** argument = allocateMemory(sizeof(void*) * 2);

     createPipe(PIPE_NAME, 4096, 0);
     int mode = 1;
     argument[0] = &mode;
     argument[1] = (void * ) "writePipe";
     pid[0] = createProcess(&writePipe, 2, argument);
     argument[1] = (void * ) "readPipe";
     pid[1] = createProcess(&readPipe, 5, argument);
     waitChild(pid[0]);
     waitChild(pid[1]);
     terminatePipe(PIPE_NAME);
     return 0;

}

int writePipe(int argumentQuantity, void ** argumentVector) {
     unsigned char message[MESSAGE_LENGTH];
     unsigned char c;
     int i = 0;
     printf("Type a message of maximum length %d and press enter:\n", MESSAGE_LENGTH);
     while((c = getchar()) != '\n' && i < MESSAGE_LENGTH) {
          putChar(c);
          message[i] = c;
          i++;
     }
     message[i] = 0;
     putChar('\n');
     writeOnPipe(PIPE_NAME, message, strLength(message));
     printf("Message written on pipe.\n");
     return 0;
}


int readPipe(int argumentQuantity, void ** argumentVector) {
     unsigned char buffer[MESSAGE_LENGTH];
     readFromPipe(PIPE_NAME, buffer, MESSAGE_LENGTH);
     printf("The message read from pipe was:\n%s", buffer);
     return 0;
}