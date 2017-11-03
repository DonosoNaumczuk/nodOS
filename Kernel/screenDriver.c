#include <screenDriver.h>

static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;

static const unsigned char BLACK = 0;
static const unsigned char BLUE = 1;
static const unsigned char GREEN = 2;
static const unsigned char CYAN = 3;
static const unsigned char RED = 4;
static const unsigned char MAGENTA = 5;
static const unsigned char BROWN = 6;
static const unsigned char LIGHT_GRAY = 7;
static const unsigned char DARK_GREY = 8;
static const unsigned char LIGHT_BLUE = 9;
static const unsigned char LIGTH_GREEN = 10;
static const unsigned char LIGTH_CYAN = 11;
static const unsigned char LIGTH_RED = 12;
static const unsigned char LIGTH_MAGENTA = 13;
static const unsigned char YELLOW = 14;
static const unsigned char WHITE = 15;

void printCharWithColor(char character, char font, char background){
	*currentVideo = character;
	currentVideo++;
	*currentVideo = font + background * 16;
	currentVideo++;
	if(currentVideo==video+width*height*2){
		moveUp();
	}
}

void moveUp(){
	int j = width*2;
	for (int i = 0; i <= width*height*2; i++){
		*(video+i) = *(video+j+i);
	}
	j=currentVideo-width-video;
	for (int i = 0; i <= width; i=i+2){
		*(video+j+i) = ' ';
		*(video+j+i+1) = 0;
	}
	currentVideo = currentVideo - width*2;
}

void printWithColor(char * string, char font, char background){
	for (int i = 0; string[i] != 0; i++){
		printCharWithColor(string[i],font,background);
	}
}

void printRainbow(char * string){
	int color = 0;
	for (int i = 0; string[i] != 0; i++){
		color = color%16;
		color = (color==0)?color+1:color;
		printCharWithColor(string[i],color,0);
		color++;
	}
}

void newLine(){
	do{
		printCharWithColor(' ',0,0);
	} while((currentVideo-video)%(width*2) != 0);
}

void clear(){
	for (int i = 0; i < height*width*2; i=i+2){
		*(video+i) = ' ';
		*(video+i+1) = 0;
	}
	currentVideo = video;
}