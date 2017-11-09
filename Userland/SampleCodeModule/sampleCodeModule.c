#include <syscall.h>
#include <cuadraticGraficator.h>
#include <lib.h>

int main() {
	printf("el numero es:%%d %s\n",20,"sabelo %dperro");
	write("hola\n",5,0x0F);
	graphInit();
	graphCuadratic(20,4,1000);
	while(1);
}
