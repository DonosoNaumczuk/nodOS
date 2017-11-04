#include <keyboard.h>
#include <naiveConsole.h> //Change to videoDriver when finished

unsigned char keycode_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* INDEX: 0 - 9 */
    '9','0', '-', '=', '\b' /* Backspace */, '\t' /* Tab */,'q', 'w', 'e', 'r',	/* INDEX: 10 - 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n'/* Enter key */, 0 /* Control */, /* INDEX: 20 - 29 */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* INDEX: 30 - 39 */
 '\'', '`',   0/* Left shift */,'\\', 'z', 'x', 'c', 'v', 'b', 'n', /* INDEX: 40 - 49 */
  'm', ',', '.', '/', 0/* Right shift */,'*',0/* Alt */,' '/* Space bar */, 0/* Caps lock */, 0 /* F1 */, /* INDEX: 50 - 59 */
    0/* F2 */,   0/* F3 */,   0/* F4 */,   0/* F5 */,   0/* F6 */,   0/* F7 */,   0/* F8 */,   0/* F9 */, 0 /*F10 */, 0/*Num lock*/, /* INDEX: 60 - 69 */
    0 /* Scroll Lock */,0 /* Home key */, 0 /* Up Arrow */, 0 /* Page Up */, '-', 0/* Left Arrow */, 0, 0/* Right Arrow */,'+', 0/*End key*/, /* INDEX: 70 - 79 */
    0/* Down Arrow */,0/* Page Down */, 0/* Insert Key */, 0/* Delete Key */, 0,   0,   0,  0/* F11 Key */, 0/* F12 Key */,
    0,	/* All other keys are undefined */
};

void keyboard_handler() {
	unsigned char status;
	char keycode;

    /* End of interrupt, PIC ack */
	write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);

	/* We want to read from data port only if lowest bit of status is 1 */
	if(status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
	}

    /* Brake keycodes are < 0 */
    if(keycode >= 0)
        ncPrintChar(keycode_map[keycode]); //Change to videoDriver when finished
}
