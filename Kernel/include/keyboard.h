#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define KEYBOARD_STATUS_PORT    0x64
#define KEYBOARD_DATA_PORT      0x60
#define BACKSPACE               128
#define ENTER_KEY               129
#define LEFT_SHIFT              130
#define RIGHT_SHIFT             131
#define CAPS_LOCK               132
#define UP_ARROW                133
#define DOWN_ARROW              134
#define LEFT_ARROW              135
#define RIGHT_ARROW             136
#define TRUE                    1
#define FALSE                   0
#define MAXCODE                 128
#define IS_LOWERCASE ((caps_lock && (left_shift || right_shift)) || (!caps_lock && !left_shift && !right_shift))
#define SHIFT_PRESSED (left_shift || right_shift)
#define IS_ARROW(k) (k == UP_ARROW || k == DOWN_ARROW || k == RIGHT_ARROW || k == LEFT_ARROW)
#define MAX_BUFF_SIZE           512
#define EOF                     -1

void keyboard_handler();
unsigned char shiftedChar(unsigned char c);
int isAlpha(unsigned char c);
int isNumber(unsigned char c);
int getchar();
void add(unsigned char c);

#endif
