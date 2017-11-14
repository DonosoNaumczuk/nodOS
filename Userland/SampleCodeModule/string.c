void strcpy(unsigned char* s1,unsigned char* s2){
	while(*s2 != 0){
		*s1 = *s2;
		s1++;
		s2++;
	}

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