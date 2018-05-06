#ifndef LIST_ADT
#define LIST_ADT

#define INSERTION_OK 0
#define REMOTION_OK 0
#define NULL_ELEMENT_ERROR 1
#define SIZE_ERROR 2
#define NULL_LIST_ERROR -3
#define EMPTY_LIST_ERROR -2
#define ELEMENT_DOESNT_EXIST -1
#define FALSE 0
#define TRUE 1
#define NULL_FUNCTION_POINTER -4

typedef struct list_t *listObject_t;

#include <stdlib.h>
#include <string.h>
#include "../../include/memoryAllocator.h"

listObject_t newList();
int addElement(listObject_t list,void *element,const unsigned int size);
int getElementOnIndex(listObject_t list,void *buffer,const unsigned int index);
int getFirstElementByCriteria(listObject_t list,int (*compareTo)(void*,void*),void *reference,void *buffer);
int getFirstElement(listObject_t list,void * buffer);
int removeElementOnIndex(listObject_t list,const unsigned int index);
int removeFirst(listObject_t list);
int size(listObject_t list);
int contains(listObject_t list,int (*compareTo)(void*,void*),void *element);

#endif
