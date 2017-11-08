#include <syscall.h>
#include <cuadraticGraficator.h>
#include <lib.h>

int main() {
	printf("el numero es:%s\n","sissssssss");
	write("hola\n",5,0x0F);
	graphInit();
	graphCuadratic(2,1,0);
	while(1);
}
