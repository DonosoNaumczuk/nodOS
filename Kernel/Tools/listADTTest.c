#include <stdio.h>
#include "./include/listADT.h"

int main(void) {
	printf("%s\n","Test Start");
	listObject_t myList = newList();
	int returnedValue;
	int getState;

	for (size_t i = 0; i < 6; i++) {
		printf("ELEMENT: %d ADDED WITH STATE: %d LIST SIZE: %d\n",i,addElement(myList,i,sizeof(int)),size(myList));
	}

	for (size_t i = 0; i < 10; i++) {
		getState = getFirstElement(myList,&returnedValue);
		printf("RETURNED FIRST: %d GETTED WITH STATE: %d LIST SIZE: %d\n",&returnedValue,getState,size(myList));
	}

	for (size_t i = 0; i < 10; i++) {
		printf("REMOVE FIRST WITH STATE: %d LIST SIZE: %d\n",removeFirst(myList),size(myList));
	}

	return 0;
}
