#include <videoDriver.h>
#include <font.h>

static const vbe_mode_info * vbeInfo = 0x5C00;
static uint8_t * framebuffer_start_address;
static uint32_t x_resolution;
static uint32_t y_resolution;
static uint32_t currentVideo_x;
static uint32_t currentVideo_y;
static uint32_t max_word_x;
static uint32_t max_word_y;

void initializeVideoDriver() {
	framebuffer_start_address = vbeInfo->framebuffer;
	x_resolution = vbeInfo->width;
	y_resolution = vbeInfo->height;
	max_word_x = x_resolution/CHAR_WIDTH;
	max_word_y = y_resolution/CHAR_HEIGHT;
	currentVideo_x = 0;
	currentVideo_y = 0;
}

uint32_t getXResolution() {
	return x_resolution;
} 

uint32_t getYResolution() {
	return y_resolution;
}

void printPixel(int x, int y, char color) {
		*(framebuffer_start_address+(-y+y_resolution/2)*x_resolution+x+x_resolution/2) = color;
}

void printCharWithColor(char character, char color) {
	if (character == '\n') {
		newLine();
		return;
	}
	char * character_font = pixel_map(character);
	for (int i = 0; i < 16; ++i) {
		printFont(framebuffer_start_address+currentVideo_x*CHAR_WIDTH+(currentVideo_y*CHAR_HEIGHT+i)*x_resolution,*(character_font+i), color);
	}
	if(currentVideo_x == max_word_x-1) {
		currentVideo_x = 0;
		if(currentVideo_y == max_word_y-1) {
			moveup();
		} else {
			currentVideo_y++;
		}
	} else {
		currentVideo_x++;
	}
}

void printWithColor(char * string, uint64_t length, char color) {
	for (int i = 0; i < length; ++i) {
		printCharWithColor(string[i],color);
	}
}

void deleteCurrent() {
	if(currentVideo_x != 0 || currentVideo_y != 0) {
		if(currentVideo_x == 0) {
			currentVideo_x = max_word_x-1;
			currentVideo_y--;
		} else {
			currentVideo_x--;
		}
		for (int i = 0; i < 16; ++i) {
			printFont(framebuffer_start_address+currentVideo_x*CHAR_WIDTH+(currentVideo_y*CHAR_HEIGHT+i)*x_resolution,XXXXXXXX,0xFF);
		}
	}

}

void printFont(uint8_t * address, char font, char color) {
	char bits[] = {font&0X80,font&0X40,font&0X20,font&0X10,font&0X08,font&0X04,font&0X02,font&0X01};
	for (int i = 0; i < 8; ++i) {
		if(bits[i]) {
			*(address+i)=color;
		}
	}
}

void moveup() {
	for (int i = CHAR_HEIGHT; i < y_resolution; ++i) {
		for (int j = 0; j < x_resolution; ++j) {
			*(framebuffer_start_address+(i-CHAR_HEIGHT)*x_resolution+j) = *(framebuffer_start_address+i*x_resolution+j);
		}
	}
	for (int i = (max_word_y-1)*CHAR_HEIGHT; i < y_resolution; ++i) {
		for (int j = 0; j < x_resolution; ++j) {
			*(framebuffer_start_address+i*x_resolution+j) = 0x00;
		}
	}
}

void newLine() {
	if(currentVideo_y!=max_word_y-1) {
		currentVideo_x = 0;
		currentVideo_y++;
	} else {
		currentVideo_x = 0;
		moveup();
	}
}

void clear() {
	for (int i = 0; i < y_resolution; ++i) {
		for (int j = 0; j < x_resolution; ++j) {
			*(framebuffer_start_address+i*x_resolution+j) = 0x00;
		}
	}
}
