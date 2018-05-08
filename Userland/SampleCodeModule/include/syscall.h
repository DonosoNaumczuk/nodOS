#ifndef _syscall_userland_h_
#define _syscall_userland_h_
#include <stdint.h>
#include <int80.h>

#define WRITE 0
#define READ 1
#define WRITE_PIXEL 2
#define GET_RESOLUTION_X 3
#define GET_RESOLUTION_Y 4
#define TIME 5
#define CLEAR 6
#define TERMINATE_PROCESS 7
#define SLEEP_PROCESS 8
#define WAIT_CHILD_PROCESS 9
#define WAKE_UP_PROCESS 10
#define CREATE_PROCESS 11
#define GET_PID 12

#define KEYBOARD_IN_ID 0
#define SCREEN_IN_ID 1

void write(char * string, uint64_t length, char color);
void read(int in_id, char * buffer, int qty_to_read);
void printPixel(uint32_t x, uint32_t y, char color);
uint64_t getResolutionX();
uint64_t getResolutionY();
uint64_t getTime(int time);
void cleanScreen();
uint64_t createProcess(void * codeAddress, uint32_t parametersQuantity, void ** parametersVector);
int waitChild(uint64_t processId);

#endif
