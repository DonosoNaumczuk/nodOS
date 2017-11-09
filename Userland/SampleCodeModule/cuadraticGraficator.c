#include <cuadraticGraficator.h>

static uint32_t x_resolution;
static uint32_t y_resolution;

#define MARK_WIDE 2
#define DOM x_resolution*escale_factor_x
#define IMG y_resolution*escale_factor_y

void graphInit(){
	x_resolution = getResolutionX();
	y_resolution = getResolutionY();
}

void graphCuadratic(int a, int b, int c){
	cleanScreen();
	int long escale_factor_x = b+100;
	int long escale_factor_y = b+100;

	if(a!=0) {
		int critic_point_x = -b/(2*a);
		int critic_point_y = a*critic_point_x*critic_point_x+b*critic_point_x+c;
		int point_x = x_resolution/4;
		int point_y = (a*point_x*point_x+b*point_x+c)/escale_factor_y;
		point_x++;
		int point_y2 = (a*point_x*point_x+b*point_x+c)/escale_factor_y;
		int dist = point_y - point_y2;
		point_x--;

		while(critic_point_x>(DOM*0.2)||critic_point_x<DOM*-(0.2)){
			escale_factor_x+=100;
		}
		while(critic_point_y>(IMG*0.2)||critic_point_y<(IMG*-0.2)||dist>2||dist<-2){
			escale_factor_y+=100;
			point_y = (a*point_x*point_x+b*point_x+c)/escale_factor_y;
			point_x++;
			point_y2 = (a*point_x*point_x+b*point_x+c)/escale_factor_y;
			dist = point_y - point_y2;
			point_x--;
		}
	}
	graphCuadraticWithScale(a,b,c,escale_factor_x,escale_factor_y);
}

void graphCuadraticWithScale(int a, int b, int c, int long escale_factor_x, int long escale_factor_y){
	int y1, y2;
	printAxiX();
	printAxiY();
	for (int i = 0; i < x_resolution/2; i++) {
		y1 = ((a*i*i)+b*i+c)/escale_factor_y;
		y2 = ((a*i*i)-b*i+c)/escale_factor_y;
		printPixel(i,y1,0x0D);
		printPixel(-i,y2,0x0D);
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