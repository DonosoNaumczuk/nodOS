#include <stdio.h>
#include "./include/listADT.h"

int intEquals(int *a,int *b);

int main(void) {
	printf("%s\n","Test Start");
	listObject_t myList = newList();
	int returnedValue;
	int getState;

	for (int i = 0; i < 6; i++) {
		printf("ADDED ELEMENT: %d STATE: %d LIST SIZE: %d\n",i,addElement(myList,&i,sizeof(int)),size(myList));
	}

	for (int i = 0; i < 11; i++) {
		getState = getElementOnIndex(myList,&returnedValue,i);
		printf("GETTED BY INDEX ELEMENT: %d ELEMENT SIZE: %d INDEX: %d LIST SIZE: %d\n",returnedValue,getState,i,size(myList));
	}

	returnedValue = -144;
	int numero = 4;
	printf("CONTAINS 4?: %s\n",(contains(myList,&intEquals,&numero)==0?"FALSE":"TRUE"));

	getFirstElementByCriteria(myList,&intEquals,&numero,&returnedValue);
	printf("GET NUMBER: %d RESULT: %d\n",numero,returnedValue);

	numero = 6;
	printf("CONTAINS 6?: %s\n",(contains(myList,&intEquals,&numero)==0?"FALSE":"TRUE"));


	for (int i = 0; i < 11; i++) {
		printf("REMOVE FIRST STATE: %d LIST SIZE: %d\n",removeFirst(myList),size(myList));
	}

	for (int i = 0; i < 6; i++) {
		printf("ELEMENT: %d ADDED WITH STATE: %d LIST SIZE: %d\n",i,addElement(myList,&i,sizeof(int)),size(myList));
	}

	for (int i = 0; i < 10; i++) {
		printf("REMOVE ON INDEX: %d STATE: %d LIST SIZE: %d\n",i,removeElementOnIndex(myList,i),size(myList));
	}

	return 0;
}

int intEquals(int *a,int *b) {
	printf("COMPARE %d CONTRA %d\n",*a,*b);
	if(a == b)	return 1;
	if(a == NULL || b == NULL) return 0;
	return *a == *b;
}
