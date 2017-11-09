#include <keyboard.h>

unsigned int keycode_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* INDEX: 0 - 9 */
    '9','0', '-', '=', BACKSPACE, '\t' /* Tab */,'q', 'w', 'e', 'r',	/* INDEX: 10 - 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', ENTER_KEY, 0 /* Control */, /* INDEX: 20 - 29 */
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
static char buffer[MAX_BUFF_SIZE];
static int current_index;
static int end_index;
static int bufferIsEmpty = TRUE;

void keyboard_handler() {
	unsigned char status;
	char keycode;

	status = read_port(KEYBOARD_STATUS_PORT);

	/* We want to read from data port only if lowest bit of status is 1 */
	if(status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);

        /* Brake codes are negative */
        if(keycode < 0) {
            if(keycode_map[keycode + MAXCODE] == RIGHT_SHIFT)
                right_shift = FALSE;
            else if(keycode_map[keycode + MAXCODE] == LEFT_SHIFT)
                left_shift = FALSE;

            return;
        }

        unsigned int mapped_key = keycode_map[keycode];
        if(mapped_key == RIGHT_SHIFT)
            right_shift = TRUE;
        else if(mapped_key == LEFT_SHIFT)
            left_shift = TRUE;
        else if(mapped_key == CAPS_LOCK)
            caps_lock = (caps_lock == TRUE)? FALSE : TRUE;
        else if(mapped_key == BACKSPACE)
            add('\b');
        else if(mapped_key == ENTER_KEY)
            add('\n');
        else if(isAlpha(mapped_key)) {
            if(IS_LOWERCASE)
                add(mapped_key);
            else
                add(shiftedChar(mapped_key));
        }
        else if(isNumber(mapped_key)) {
            if(SHIFT_PRESSED)
                add(shiftedChar(mapped_key));
            else
                add(mapped_key);
        }
    }
}

char shiftedChar(char c) {
    char shifted;
    if(isAlpha)
        return c - 32;
    switch (c) {
        case '1':
            shifted = '!';
            break;
        case '2':
            shifted = '@';
            break;
        case '3':
            shifted = '#';
            break;
        case '4':
            shifted = '$';
            break;
        case '5':
            shifted = '%';
            break;
        case '6':
            shifted = '^';
            break;
        case '7':
            shifted = '&';
            break;
        case '8':
            shifted = '*';
            break;
        case '9':
            shifted = '(';
            break;
        case '0':
            shifted = ')';
            break;
        case '-':
            shifted = '_';
            break;
        case '=':
            shifted = '+';
            break;
        case '[':
            shifted = '{';
            break;
        case ']':
            shifted = '}';
            break;
        case '\'':
            shifted = '"';
            break;
        case ';':
            shifted = ':';
            break;
        case '/':
            shifted = '?';
            break;
        case ',':
            shifted = '<';
            break;
        case '.':
            shifted = '>';
            break;
        default:
            shifted = 0;
    }
    return shifted;
}

int isAlpha(char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int isNumber(char c) {
    return (c >= '0' && c <= '9');
}

int getchar() {
    _sti();
    while (bufferIsEmpty);

    int c = buffer[current_index];

    if(++current_index > end_index) {
        bufferIsEmpty = TRUE;
    }

    return c;
}

void add(char c) {
    if(bufferIsEmpty) {
        current_index = 0;
        end_index = 0;
        bufferIsEmpty = FALSE;
    }
    else {
        end_index = (end_index + 1) % MAX_BUFF_SIZE;
        if(end_index == current_index)
            current_index++;
    }
    buffer[end_index] = c;
}
