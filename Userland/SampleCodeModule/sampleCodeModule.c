#include <syscall.h>
#include <cuadraticGraficator.h>

int main() {
	graphInit();
	graphCuadratic(2,1,0);
	while(1);
}
