#include <string.h>

void strcpy(unsigned char *s1, unsigned char *s2){
	while(*s2 != 0){
		*s1 = *s2;
		s1++;
		s2++;
	}
	*s1 = 0;

}

void strncpy(unsigned char *s1, unsigned char *s2, unsigned int beginning, unsigned int end) {
	unsigned int i = 0;
	while(beginning < end && s2[beginning] != 0) {
		s1[i] = s2[beginning];
		i++;
		beginning ++;
	}
	s1[i] = 0;
}


int strcmp(unsigned char* s1,unsigned char* s2){
	int res;
	res = 0;
	
	while(*s1 != 0 || *s2 != 0){
		if((res += *s1 - *s2) != 0)	return	res;
		if(*s1 != 0)	s1++;
		if(*s2 != 0)	s2++;
	}
	if(!((*s1 == 0)&&(*s2 == 0)))	return	-1;
	return	res;
}

int strncmp(unsigned char* s1,unsigned char* s2,unsigned int n){
	int res;
	res = 0;
	
	while(n > 0){
		if((res += *s1 - *s2) != 0)	return	res;	
		if(*s1 != 0)	s1++;
		if(*s2 != 0)	s2++;
		n--;
	}
	return	res;
}

int strLength(unsigned char *string) {
	int i = 0;
	while(string[i] != 0) {
		i++;
	}
	return i;
}

void reverseString(unsigned char *string) {
	int i = strLength(string) - 1;
	int limit = i / 2;
	int beginning = 0;
	unsigned char aux;
	while(i > limit) {
		aux = string[beginning];
		string[beginning] = string[i];
		string[i] = aux;
		i--;
		beginning++; 
	}
}