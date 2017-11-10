#include <syscall.h>
#include <cuadraticGraficator.h>
#include <lib.h>

int main() {
	int n1;
	char string[100];
	while(1){
		printf("La cantidad leida es: %d\n",scanf("el numero es %d y el string es %s",&n1,string));
		printf("El numero es: %d\n",n1);
		printf("El string es: %s\n",string);
	}
}
