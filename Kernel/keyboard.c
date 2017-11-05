#include <keyboard.h>
#include <videoDriver.h>

unsigned int keycode_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* INDEX: 0 - 9 */
    '9','0', '-', '=', BACKSPACE, '\t' /* Tab */,'q', 'w', 'e', 'r',	/* INDEX: 10 - 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', ENTER, 0 /* Control */, /* INDEX: 20 - 29 */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* INDEX: 30 - 39 */
 '\'', '`',  LEFT_SHIFT,'\\', 'z', 'x', 'c', 'v', 'b', 'n', /* INDEX: 40 - 49 */
  'm', ',', '.', '/', LEFT_SHIFT,'*',0/* Alt */,' '/* Space bar */, CAPS_LOCK, 0 /* F1 */, /* INDEX: 50 - 59 */
    0/* F2 */,   0/* F3 */,   0/* F4 */,   0/* F5 */,   0/* F6 */,   0/* F7 */,   0/* F8 */,   0/* F9 */, 0 /*F10 */, 0/*Num lock*/, /* INDEX: 60 - 69 */
    0 /* Scroll Lock */,0 /* Home key */, 0 /* Up Arrow */, 0 /* Page Up */, '-', 0/* Left Arrow */, 0, 0/* Right Arrow */,'+', 0/*End key*/, /* INDEX: 70 - 79 */
    0/* Down Arrow */,0/* Page Down */, 0/* Insert Key */, 0/* Delete Key */, 0,   0,   0,  0/* F11 Key */, 0/* F12 Key */,
    0,	/* All other keys are undefined */
};

unsigned static char caps_lock = FALSE;
unsigned static char left_shift = FALSE;
unsigned static char right_shift = FALSE;
#define ISLOWERCASE ((caps_lock && (left_shift || right_shift)) || (!caps_lock && !left_shift && !right_shift))

void keyboard_handler() {
	unsigned char status;
	char keycode;

    /* End of interrupt, PIC ack */
	write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);

	/* We want to read from data port only if lowest bit of status is 1 */
	if(status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);

        if(keycode_map[keycode] == RIGHT_SHIFT)
            right_shift = TRUE;
        else if(keycode_map[keycode] == LEFT_SHIFT)
            left_shift = TRUE;
        else if(keycode < 0 && keycode_map[keycode + MAXCODE] == RIGHT_SHIFT)
            right_shift = FALSE;
        else if(keycode < 0 && keycode_map[keycode + MAXCODE] == LEFT_SHIFT)
            left_shift = FALSE;
        else if(keycode_map[keycode] == CAPS_LOCK)
            caps_lock = (caps_lock == TRUE)? FALSE : TRUE;
        else if(keycode_map[keycode] == BACKSPACE)
            deleteCurrent();
        else if(keycode_map[keycode] == ENTER)
            newLine();
        else if(keycode >= 0) { /* Brake keycodes are < 0 */
            if(ISLOWERCASE)
                printChar(keycode_map[keycode]);
            else
                printChar(keycode_map[keycode] - 32);
        }
	}
}
