#include <cuadraticGraficator.h>

static uint32_t x_resolution;
static uint32_t y_resolution;

#define MARK_WIDE 2

void graphInit(){
	x_resolution = getResolutionX();
	y_resolution = getResolutionY();
}

void graphCuadratic(int a, int b, int c){
	cleanScreen();
	int escale_factor_x, escale_factor_y;
	if(a==0) {
		escale_factor_x = 100;
		escale_factor_y = escale_factor_x;
	} else {
		escale_factor_x = a^100+b^50+c^25;
		escale_factor_y = escale_factor_x;
	}

	int i;
	int y1, y2;
	printAxiX();
	printAxiY();
	for (int j = 0; j < x_resolution*escale_factor_x/2; j++) {
		i = j/escale_factor_x;
		y1 = ((a*i*i)+b*i+c)/escale_factor_y;
		y2 = ((a*i*i)-b*i+c)/escale_factor_y;
		printPixel(i,y1,0x0A);
		printPixel(-i,y2,0x0A);
	}
	printf("Cada linea en el eje X son %d valores\n",escale_factor_x*10);
	printf("Cada linea en el eje Y son %d valores\n",escale_factor_y*10);
}

void estrellita(){
	int a = -9;
	int b = 100;
	int c = 100;
	cleanScreen();
	int escale_factor_x, escale_factor_y;
	escale_factor_x = 10^(a^2);
	escale_factor_y = escale_factor_x;
	/*if(a <= 10 && a >= -10){
		escale_factor_x = 100;
		escale_factor_y = 100;
	} else{
		escale_factor_x = 10;
		escale_factor_y = escale_factor_x;
	}*/
	int i;
	int y1, y2;
	for (int j = 0; j < x_resolution*escale_factor_x/2; j++) {
		i = j/escale_factor_x;
		y1 = ((a*i*i)+b*i+c)/escale_factor_y;
		y2 = ((a*i*i)-b*i+c)/escale_factor_y;
		printPixel(i,y1,0x0A);
		printPixel(-i,y2,0x0A);
	}
	printAxiX();
	printAxiY();
	printf("Cada linea en el eje X son %d valores\n",escale_factor_x*10);
	printf("Cada linea en el eje Y son %d valores\n",escale_factor_y*10);
}

void printAxiX(){
	for (int i = 0; i < x_resolution/2; ++i) {
		printPixel(i,0,0x0F);
		printPixel(-i,0,0x0F);
		if(!(i%10)) {
			for (int j = 1; j <= MARK_WIDE; ++j) {
				printPixel(i,j,0x0F);
				printPixel(-i,-j,0x0F);
				printPixel(i,-j,0x0F);
				printPixel(-i,j,0x0F);
			}
		}
	}
}

void printAxiY(){
	for (int i = 0; i < x_resolution/2; ++i) {
		printPixel(0,i,0x0F);
		printPixel(0,-i,0x0F);
		if(!(i%10)) {
			for (int j = 1; j <= MARK_WIDE; ++j) {
				printPixel(j,i,0x0F);
				printPixel(-j,-i,0x0F);
				printPixel(j,-i,0x0F);
				printPixel(-j,i,0x0F);
			}
		}
	}
}