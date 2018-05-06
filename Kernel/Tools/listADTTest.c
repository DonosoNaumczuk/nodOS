#include <stdio.h>
#include "./include/listADT.h"

int main(void) {
	printf("%s\n","Test Start");
	listObject_t myList = newList();
	int returnedValue;
	int getState;

	for (int i = 0; i < 6; i++) {
		printf("ELEMENT: %d ADDED WITH STATE: %d LIST SIZE: %d\n",i,addElement(myList,&i,sizeof(int)),size(myList));
	}

	for (int i = 0; i < 10; i++) {
		getState = getElementOnIndex(myList,&returnedValue,i);
		printf("ELEMENT: %d ELEMENT SIZE: %d INDEX: %d LIST SIZE: %d\n",returnedValue,getState,i,size(myList));
	}

	for (int i = 0; i < 10; i++) {
		printf("REMOVE FIRST STATE: %d LIST SIZE: %d\n",removeFirst(myList),size(myList));
	}

	return 0;
}
