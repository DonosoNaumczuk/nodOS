#include <cuadraticGraficator.h>

static uint32_t x_resolution;
static uint32_t y_resolution;

void graphInit(){
	x_resolution = getResolutionX();
	y_resolution = getResolutionY();
}

void graphPoli(int a, int b, int c){
	if(a == 0)
		graphLinear(b,c);
	else
		graphCuadratic(a,b,c);
}

void graphCuadratic(int a, int b, int c){
	//int critic_point_x = b/(a*2);
	//int critic_point_y = (a*(critic_point_x*critic_point_x)+b*critic_point_x+c);

	/*if(!printAxis(critic_point_x,critic_point_y)){
		center_point_x = 0;
		critic_point_y = 0;
	}*/
	int y1;
	int y2;
	for (int i = 0; i < x_resolution/2; i++) {
		y1 = (a*i*i)+b*i+c;
		y2 = (a*i*i)-b*i+c;
		printPixel(i,y1,0x0F);
		printPixel(-i,y2,0x0F);
	}

}

void graphLinear(int a, int b){
	return;
}

/*int printAxis(int center_point_x, int center_point_y){
	int aux_x = 0;
	int aux_y = 0;

	if(mod(critic_point_x) > x_resolution-100) 
		aux_x = center_point_x;

	if(mod(center_point_y) > y_resolution-100)
		aux_y = critic_point_y;

	printAxiX(aux_x);
	printAxiY(aux_y);
}*/