#include <videoDriver.h>
#include <font.h>

static const vbe_mode_info * vbeInfo = 0x5C00;
static uint8_t * framebuffer_start_address;
static uint16_t width;
static uint16_t height;
static uint8_t currentVideo_x = 0;
static uint8_t currentVideo_y = 0;
static const uint32_t width_letters = 80;
static const uint32_t height_letters = 25;

void initializeVideoDriver() {
	static int initialized = 0;
	if(!initialized) {
		framebuffer_start_address = vbeInfo->framebuffer;
		width = vbeInfo->width;
		height = vbeInfo->height;
		initialized = 1;
	}
}

void printCharWithColor(char character, char color){
	char * character_font = pixel_map(character);
	for (int i = 0; i < 16; ++i){
		printFont(framebuffer_start_address+currentVideo_x*CHAR_WIDTH+(currentVideo_y*CHAR_HEIGHT+i)*width,*(character_font+i), color);
	}
	if(currentVideo_x == width_letters-1){
		currentVideo_x = 0;
		if(currentVideo_y == height_letters-1){
			moveup();
		} else{
			currentVideo_y++;
		}
	} else{
		currentVideo_x++;
	}
}

void deleteCurrent(){
	if(currentVideo_x != 0 || currentVideo_y != 0){
		if(currentVideo_x == 0){
			currentVideo_x = width_letters-1;
			currentVideo_y--;
		} else{
			currentVideo_x--;
		}
		for (int i = 0; i < 16; ++i){
			printFont(framebuffer_start_address+currentVideo_x*CHAR_WIDTH+(currentVideo_y*CHAR_HEIGHT+i)*width,XXXXXXXX,0xFF);
		}
	}
	
}

void printFont(uint8_t * address, char font, char color){
	char bits[] = {font&0X80,font&0X40,font&0X20,font&0X10,font&0X08,font&0X04,font&0X02,font&0X01};
	for (int i = 0; i < 8; ++i){
		if(bits[i]){
			*(address+i)=color;
		}
	}
}

void printWithColor(char * string, char color){
	for (int i = 0; string[i] != 0; ++i){
		printCharWithColor(string[i],color);
	}
}

void moveup(){
	for (int i = CHAR_HEIGHT; i < height; ++i){
		for (int j = 0; j < width; ++j){
			*(framebuffer_start_address+(i-CHAR_HEIGHT)*width+j) = *(framebuffer_start_address+i*width+j);
		}
	}
	for (int i = (height_letters-1)*CHAR_HEIGHT; i < height; ++i){
		for (int j = 0; j < width; ++j){
			*(framebuffer_start_address+i*width+j) = 0x00;
		}
	}
}

void newLine(){
	currentVideo_x = 0;
	currentVideo_y++;
}

void clear(){
	for (int i = 0; i < height; ++i){
		for (int j = 0; j < width; ++j){
			*(framebuffer_start_address+i*width+j) = 0x00;
		}
	}
}
