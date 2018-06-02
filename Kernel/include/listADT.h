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

#include <lib.h>
#include <memoryAllocator.h>

listObject_t newList();

/* Receive a list, an element to add and the element size
   Return NULL_LIST_ERROR if the list is NULL
   Return NULL_ELEMENT_ERROR if the element is NULL
   Return SIZE_ERROR if the size is smaller than 1
   Return INSERTION_OK if the element was inserted to the list
*/
int addElement(listObject_t list,const void *element,const unsigned int size);

int getElementOnIndex(listObject_t list,void *buffer,const unsigned int index);

int getFirstElementByCriteria(listObject_t list,int (*compareTo)(void*,void*),const void *reference,void *buffer);

int getFirstElement(listObject_t list,void * buffer);

void *getFirstElementReferenceByCriteria(listObject_t list,int (*compareTo)(void*,void*),const void *reference);

void *getFirstElementReferece(listObject_t list);

// int removeElementOnIndex(listObject_t list,const unsigned int index);

int size(listObject_t list);

int contains(listObject_t list, int (*compareTo)(void * , void * ), const void *reference);

int removeFirst(listObject_t list);

int removeAndFreeFirst(listObject_t list);

int removeFirstElementByCriteria(listObject_t list,int (*compareTo)(void*,void*),const void *reference);

int removeAndFreeFirstElementByCriteria(listObject_t list,int (*compareTo)(void*,void*),const void *reference);

int removeAllElements(listObject_t list);

int removeAndFreeAllElements(listObject_t list);

void freeList();

#endif
