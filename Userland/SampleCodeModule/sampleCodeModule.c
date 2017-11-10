#include <syscall.h>
#include <cuadraticGraficator.h>
#include <lib.h>

int main() {
	int a=0,b=0,c=0;
	char string[100];
	graphInit();
	while(1){
		if(3==scanf("%d %d %d",&a,&b,&c))
			graph(a,b,c);
		else printf("NO\n");
	}
}
