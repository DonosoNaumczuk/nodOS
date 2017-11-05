#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60
#define BACKSPACE 258
#define ENTER 259
#define LEFT_SHIFT 260
#define RIGHT_SHIFT 261
#define CAPS_LOCK 262
#define TRUE 1
#define FALSE 0
#define MAXCODE 128

void keyboard_handler();

#endif
