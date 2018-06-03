#include <lib.h>

#define MAX 100

static int color = 0x0F;

unsigned char putChar(char c){
	write(&c, 1, color);
	return c;
}

int getchar() {
	char c;
	read(0, &c, 1);
	return c;
}

int printf(const char* c, ...) {
	va_list args;
	va_start(args,c);

	//int ret = 0; // return
	char aux[MAX];
	int j = 0;
	int flag = 0;
	int i = 0;
	while(c[i]!=0){
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
		if(j==MAX){
			//aux[j];
			write(aux,MAX,color);
			j = 0;
		}
		i++;
	}
	va_end(args);
	write(aux,j,color);
	return 0;
}

int scanf(const char* c, ...){
	va_list args;
	va_start(args,c);

	int ret = 0;
	int flag = 0;
	int i = 0;
	char buffer[MAX];
	int j = 0;
	char currentChar;

	while((currentChar = getchar()) != '\n'){
		if(currentChar == '\b'){
			if(j>0){
				buffer[--j] = 0;
				printf("\b");
			}
		}else{
			if(j < MAX){
				buffer[j++]	= currentChar;
				putChar(currentChar);
			}
		}
	}
	buffer[j] = 0;
	j = 0;

	while(c[i]!=0&&ret>=0){
		switch(c[i]) {
			case '%':
				if(flag) {
					if(buffer[j]!='%')
						ret = -1;
					else
						j++;
				} else {
					flag = 1;
				}
				break;
			case 'd':
				if(flag){
					if(getNumber(va_arg(args,int*),buffer,&j))
						ret++;
					else
						ret = -1;
					flag = 0;
				} else {
					if(buffer[j]!='d')
						ret = -1;
					else
						j++;
				}
				break;
			case 's':
			if(flag){
					j += copyString(buffer+j, va_arg(args,char*));
					ret++;
					flag = 0;
				} else {
					if(buffer[j]!='s')
						ret = -1;
					else
						j++;
				}
				break;
			default:
				if(buffer[j]!=c[i])
					ret = -1;
				else
					j++;
		}
		i++;
	}
	printf("\n");
	va_end(args);
	return ret;
}

int isNumber(char n) {
	if(n=='1'||n=='2'||n=='3'||n=='4'||n=='5'||n=='6'||n=='7'||n=='8'||n=='9'||n=='0')
		return 1;
	return 0;
}

int getNumber(int * dir, char * buffer, int * j) {
	int aux = 0;
	char c;
	int flag = 1;
	int is_positive = 1;
	do{
		c = buffer[*j];
		if(isNumber(c)){
			*dir = (*dir)*10;
			*dir += c - '0';
			aux = 1;
			*j = (*j)+1;
		} else if(c=='-'&&is_positive) {
			is_positive = 0;
			*j = (*j)+1;
		} else {
			flag = 0;
		}
	} while(flag);
	if(!is_positive)
		*dir = (*dir)*-1;
	return aux;
}

int toBase(int n, char* pnt, int base){
	char *p = pnt;
	char *p1, *p2;
	int digits = 0;
	int flag = 0;
	if(n<0){
		flag = 1;
		n = -n;
	}
	do
	{
		int remainder = n % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (n /= base);

	if(flag){
		*p++='-';
		digits++;
	}

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

int copyString(char* string, char* to) {
	int i;
	for (i = 0; string[i] != 0; ++i) {
		to[i] = string[i];
	}
	to[i] = 0;
	return i;
}

int pow(int base,unsigned int exponent) {
	if(exponent == 0)	return	1;
	while(exponent > 1) {
		base*=base;
		exponent--;
	}
	return base;
}

int	charToInt(unsigned char* string) {
	unsigned int digits = 0;
	unsigned int index  = 0;
	int res = 0;
	while((string[digits] != 0) && (string[digits] != ' '))	digits++;
	while(index < (digits - 1)){
		res += (string[digits - index - 1] - '0')*pow(10,index);
		index++;
	}
	if(string[digits - index - 1] == '-')	res = res*(-1);
	else 	res +=	(string[digits - index - 1] - '0')*pow(10,index);
	return res;
}

void intToString(unsigned char *destination, int number) {
	int i = 0;
	if(number == 0) {
		destination[0] = '0';
		destination[1] = 0;
	}
	while(number > 0) {
		destination[i] = (number % 10) + '0';
		i++;
		number = number / 10;
	}
	destination[i] = 0;
	reverseString(destination);
}

void changeFontColor(int newColor) {
	color = newColor;
}
