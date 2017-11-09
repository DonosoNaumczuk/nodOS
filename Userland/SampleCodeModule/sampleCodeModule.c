#include <syscall.h>
#include <cuadraticGraficator.h>
#include <lib.h>

int main() {
	int i = 20;
	while(i > 0) {
		char c = getchar();
		write(&c,1,0x0F);
		i--;
	}
	write("finish", 6, 0x0F);
	// graphInit();
	// graphCuadratic(2,1,0);
	while(1);
}
