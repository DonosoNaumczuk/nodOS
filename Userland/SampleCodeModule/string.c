

void strcpy(char* s1, char* s2){
	while(*s2 != 0){
		*s1 = *s2;
		s1++;
		s2++;
	}

}


int strcmp(char* s1, char* s2){
	int res;
	res = 0;
	
	while(*s1 != 0 || *s2 != 0){
		res += *s1 - *s2;
		if(*s1 != 0)	s1++;
		if(*s2 != 0)	s2++;
	}
	return	res;
}