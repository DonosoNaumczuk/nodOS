#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60
#define BACKSPACE 258
#define ENTER_KEY 259
#define LEFT_SHIFT 260
#define RIGHT_SHIFT 261
#define CAPS_LOCK 262
#define TRUE 1
#define FALSE 0
#define MAXCODE 128
#define IS_LOWERCASE ((caps_lock && (left_shift || right_shift)) || (!caps_lock && !left_shift && !right_shift))
#define SHIFT_PRESSED (left_shift || right_shift)
#define MAX_BUFF_SIZE 256
#define EOF -1

void keyboard_handler();
char shiftedChar(char c);
int isAlpha(char c);
int isNumber(char c);
int getchar();
void add(char c);

#endif
