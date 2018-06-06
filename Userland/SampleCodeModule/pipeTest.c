#include <pipeTest.h>
#include <stdint.h>
#include <syscall.h>
#include <pipe.h>
#include <lib.h>
#include <string.h>
#include <null.h>
#include <colors.h>

int pipe(int argumentQuantity, void **argumentVector) {
     uint64_t pid[2];
     void ** argument = allocateMemory(sizeof(void *) * 4);
     createPipe(PIPE_NAME, 4096, 0);
     int mode = 1;
     argument[0] = &mode;
     argument[1] = (void * ) "writePipe";
     argument[2] = NULL;
     argument[3] = NULL;
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
     changeFontColor(ORANGE);
     printf("Type a message of maximum length %d and press enter:\n", MESSAGE_LENGTH);
     changeFontColor(WHITE);
     while((c = getchar()) != '\n' && i < MESSAGE_LENGTH) {
          putChar(c);
          message[i] = c;
          i++;
     }
     message[i] = 0;
     putChar('\n');
     writeOnPipe(PIPE_NAME, message, strLength(message) + 1);
     changeFontColor(ORANGE);
     printf("Message written on pipe.\n");
     changeFontColor(WHITE);
     return 0;
}


int readPipe(int argumentQuantity, void ** argumentVector) {
     unsigned char buffer[MESSAGE_LENGTH];
     readFromPipe(PIPE_NAME, buffer, MESSAGE_LENGTH);
     changeFontColor(AQUAMARINE);
     printf("The message read from pipe was:\n");
     changeFontColor(WHITE);
     printf("%s\n", buffer);
     return 0;
}
