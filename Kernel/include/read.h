#ifndef _read_h_
#define _read_h_

#include <videoDriver.h>
#include <keyboard.h>
#include <scheduler.h>

#define KEYBOARD_IN_ID 0
#define SCREEN_IN_ID 1

void read(int in_id, unsigned char * buffer, int buffer_size);

#endif
