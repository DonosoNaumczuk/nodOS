#include <syscall.h>
#include <cuadraticGraficator.h>
#include <lib.h>

int main() {
	char c;
	while( (c = getchar()) != '\n' ) {
		write(&c, 1, 0x0F);
	}
	write("Finish", 6, 0x0F);
	while(1);
}
