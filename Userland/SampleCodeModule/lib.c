#include <lib.h>

#define block 10

char putChar(char c){
	write(&c,1,0x0F);
	return c;
}

int getchar() {
	char c;
	read(0, &c, 1);
	return c;
}

int printf(const char* c, ...){
	va_list args;
	va_start(args,c);

	int ret = 0; // return
	char* aux = malloc(block*sizeof(char));
	int j = 0;
	int flag = 0;
	int i = 0;
	while(c[i]!=0){
		if(!(j%(10))) {
			malloc(block*sizeof(char));
		}
		switch(c[i]) {
			case '%':
				if(flag) {
					aux[j] = '%';
					j++;
				} else {
					flag = 1;
				}
				break;
			case 'd':
				if(flag){
					j += toBase(va_arg(args,int),aux+j,10);
					flag = 0;
				} else {
					aux[j] = 'd';
					j++;
				}
				break;
			case 's':
			if(flag){
					j += copyString(va_arg(args,char*),aux+j);
					flag = 0;
				} else {
					aux[j] = 's';
					j++;
				}
				break;
			default:
				aux[j] = c[i];
				j++;
		}
		i++;
	}
	va_end(args);
	free();
	write(aux,j,0x0F);
}

int toBase(int n, char* pnt, int base){
	char *p = pnt;
	char *p1, *p2;
	int digits = 0;

	do
	{
		int remainder = n % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (n /= base);

	p1 = pnt;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

int copyString(char* string, char* to){
	int i;
	for (i = 0; string[i] != 0; ++i) {
		to[i] = string[i];
	}
	return i;
}
