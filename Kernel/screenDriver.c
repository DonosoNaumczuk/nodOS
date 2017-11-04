#include <screenDriver.h>
#include <font.h>

static uint8_t * const video = (uint8_t*)0xA0000;
static uint8_t  currentVideo_x = 0;
static uint8_t  currentVideo_y = 0;
static const uint32_t width = 640;
static const uint32_t height = 400;
static const uint32_t width_letters = 80;
static const uint32_t height_letters = 25;

void printCharWithColor(char character, char color){
	char * character_font = pixel_map(character);
	for (int i = 0; i < 16; ++i){
		printFont(video+currentVideo_x*CHAR_WIDTH+(currentVideo_y*CHAR_HEIGHT+i)*width,*(character_font+i), color);
	}
	if(currentVideo_x == width_letters-1){
		currentVideo_x = 0;
		if(currentVideo_y == height_letters-1){
			currentVideo_y--;
			//moveup();
		} else{
			currentVideo_y++;
		}
	} else{
		currentVideo_x++;
	}
}

void printFont(uint8_t * adress, char font, char color){
	char bits[] = {font&0X80,font&0X40,font&0X20,font&0X10,font&0X08,font&0X04,font&0X02,font&0X01};
	for (int i = 0; i < 8; ++i){
		if(bits[i]){
			*(adress+i)=color;
		}
	}
}