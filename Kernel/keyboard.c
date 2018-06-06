#include <keyboard.h>
#include <videoDriver.h>
#include <portIO.h>
#include <interrupts.h>
#include <scheduler.h>

unsigned char keycode_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* INDEX: 0 - 9 */
    '9','0', '-', '=', BACKSPACE, '\t' /* Tab */,'q', 'w', 'e', 'r',	/* INDEX: 10 - 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', ENTER_KEY, CONTROL /* Control */, /* INDEX: 20 - 29 */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* INDEX: 30 - 39 */
 '\'', '`',  LEFT_SHIFT,'\\', 'z', 'x', 'c', 'v', 'b', 'n', /* INDEX: 40 - 49 */
  'm', ',', '.', '/', RIGHT_SHIFT,'*',0/* Alt */,' '/* Space bar */, CAPS_LOCK, 0 /* F1 */, /* INDEX: 50 - 59 */
    0/* F2 */,   0/* F3 */,   0/* F4 */,   0/* F5 */,   0/* F6 */,   0/* F7 */,   0/* F8 */,   0/* F9 */, 0 /*F10 */, 0/*Num lock*/, /* INDEX: 60 - 69 */
    0 /* Scroll Lock */,0 /* Home key */, UP_ARROW /* Up Arrow */, 0 /* Page Up */, '-', LEFT_ARROW/* Left Arrow */, 0, RIGHT_ARROW/* Right Arrow */,'+', 0/*End key*/, /* INDEX: 70 - 79 */
    DOWN_ARROW/* Down Arrow */,0/* Page Down */, 0/* Insert Key */, 0/* Delete Key */, 0,   0,   0,  0/* F11 Key */, 0/* F12 Key */,
    0,	/* All other keys are undefined */
};

static int caps_lock = FALSE;
static int left_shift = FALSE;
static int right_shift = FALSE;
static int control = FALSE;
static unsigned char buffer[MAX_BUFF_SIZE];
static unsigned int current_index;
static unsigned int end_index;
static int bufferIsEmpty = TRUE;

void keyboard_handler() {
	char status;
	char keycode;

	status = read_port(KEYBOARD_STATUS_PORT);

	/* We want to read from data port only if lowest bit of status is 1 */
	if(status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
        /* Brake codes are negative */
        if(keycode < 0) {
            if(keycode_map[keycode + MAXCODE] == RIGHT_SHIFT) {
                right_shift = FALSE;
            }
            else if(keycode_map[keycode + MAXCODE] == LEFT_SHIFT) {
                left_shift = FALSE;
            }
            else if(keycode_map[keycode + MAXCODE] == CONTROL) {//evans
                control = FALSE;
            }
            return;
        }

        unsigned char mapped_key = keycode_map[(int) keycode];

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
        else if(IS_ARROW(mapped_key))
            add(mapped_key);
        else if(mapped_key == CONTROL) {
            control = TRUE;
        }
        else if(isAlpha(mapped_key)) {
            // if((mapped_key == 'c' || mapped_key == 'C') && control) {

            //     terminateAProcessByPid(getProcessId());
            // }
            // else 
            if(IS_LOWERCASE) {
                add(mapped_key);
            }
            else {
                add(shiftedChar(mapped_key));
            }
        }
        
        else if(isNumber(mapped_key)) {
            if(SHIFT_PRESSED && ((mapped_key = shiftedChar(mapped_key)) != 0) )
                add(mapped_key);
            else
                add(mapped_key);
        }
        else if(mapped_key != 0) {
            if(SHIFT_PRESSED && ((mapped_key = shiftedChar(mapped_key)) != 0) )
                add(mapped_key);
            else
                add(mapped_key);
        }
    }
}

unsigned char shiftedChar(unsigned char c) {
    unsigned char shifted;
    if(isAlpha(c))
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
        case '`':
            shifted = '~';
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
        case '\\':
            shifted = '|';
            break;
        default:
            shifted = 0;
    }
    return shifted;
}

int isAlpha(unsigned char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int isNumber(unsigned char c) {
    return (c >= '0' && c <= '9');
}

static unsigned int cursorNumber;

int getchar() {
    _sti();
    while (bufferIsEmpty){
        if(cursorNumber%100000000 == 0)  cursorBlink();
        cursorNumber++;
    }

    unsigned char c = buffer[current_index];

    if(++current_index > end_index) {
        bufferIsEmpty = TRUE;
    }
    if(cursorIsOn())    clearCursor();
    return c;
}

void add(unsigned char c) {
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
