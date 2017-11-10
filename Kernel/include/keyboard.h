#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60
#define BACKSPACE 128
#define ENTER_KEY 129
#define LEFT_SHIFT 130
#define RIGHT_SHIFT 140
#define CAPS_LOCK 150
#define UP_ARROW 160
#define DOWN_ARROW 170
#define LEFT_ARROW 180
#define RIGHT_ARROW 190
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
void add(char c);

#endif
