#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60
#define BACKSPACE 258
#define ENTER_KEY 259
#define LEFT_SHIFT 260
#define RIGHT_SHIFT 261
#define CAPS_LOCK 262
#define UP_ARROW 263
#define DOWN_ARROW 264
#define LEFT_ARROW 265
#define RIGHT_ARROW 266
#define TRUE 1
#define FALSE 0
#define MAXCODE 128
#define IS_LOWERCASE ((caps_lock && (left_shift || right_shift)) || (!caps_lock && !left_shift && !right_shift))
#define SHIFT_PRESSED (left_shift || right_shift)
#define IS_ARROW(k) (k == UP_ARROW || k == DOWN_ARROW || k == RIGHT_ARROW || k == LEFT_ARROW)
#define MAX_BUFF_SIZE 512
#define EOF -1

void keyboard_handler();
char shiftedChar(char c);
int isAlpha(char c);
int isNumber(char c);
int getchar();
void add(unsigned int c);

#endif
