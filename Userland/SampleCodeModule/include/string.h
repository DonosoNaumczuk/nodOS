#ifndef	_string_h_
#define	_string_h_

void	strcpy(unsigned char* s1, unsigned char* s2);
void strncpy(unsigned char *s1, unsigned char *s2, unsigned int beginning, unsigned int end);
int strcmp(unsigned char* s1, unsigned char* s2);
int strncmp(unsigned char* s1,unsigned char* s2,unsigned int n);
int strLength(unsigned char *string);

#endif