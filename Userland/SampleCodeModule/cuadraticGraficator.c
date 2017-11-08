#include <cuadraticGraficator.h>

static uint32_t x_resolution;
static uint32_t y_resolution;

void graphInit(){
	x_resolution = getResolutionX();
	y_resolution = getResolutionY();
}

void graphPoli(float a, float b, float c){
	if(a == 0)
		graphLinear(b,c);
	else
		graphCuadratic(a,b,c);
}

void graphCuadratic(float a, float b, float c){
	//float critic_pofloat_x = b/(a*2);
	//float critic_pofloat_y = (a*(critic_pofloat_x*critic_pofloat_x)+b*critic_pofloat_x+c);

	/*if(!prfloatAxis(critic_pofloat_x,critic_pofloat_y)){
		center_pofloat_x = 0;
		critic_pofloat_y = 0;
	}*/
	float y1;
	float y2;
	for (float i = 0; i < x_resolution/2; i++) {
		y1 = (a*i*i)+b*i+c;
		y2 = (a*i*i)-b*i+c;
		printPixel(i,y1,0x0F);
		printPixel(-i,y2,0x0F);
	}

}

void graphLinear(float a, float b){
	return;
}

/*float prfloatAxis(float center_pofloat_x, float center_pofloat_y){
	float aux_x = 0;
	float aux_y = 0;

	if(mod(critic_pofloat_x) > x_resolution-100) 
		aux_x = center_pofloat_x;

	if(mod(center_pofloat_y) > y_resolution-100)
		aux_y = critic_pofloat_y;

	prfloatAxiX(aux_x);
	prfloatAxiY(aux_y);
}*/