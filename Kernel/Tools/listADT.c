#include "./include/listADT.h"

typedef struct nodeStruct_t{
    unsigned int index;
    unsigned int size;
    void *element;
    struct node_t *next;
};

typedef struct list_t{
    struct node_t *head;
    int size;
};

typedef struct nodeStruct_t *node_t;

listObject_t newList() {
	listObject_t list = malloc(sizeof(struct list_t)); //EVANS
	list->size = 0;
	return list;
}

int addElement(listObject_t list,void *element,const unsigned int size) {
    if(list == NULL) return NULL_LIST_ERROR;
    if(element == NULL) return NULL_ELEMENT_ERROR;
    if(size == 0)  return SIZE_ERROR;
    node_t newNode = malloc(sizeof(node_t));   //EVANS
    newNode->element = malloc(size);            //EVANS
    newNode->size = size;
    memcpy(newNode->element,element,size);      //EVANS
    if(list->head == NULL) {
        list->head = newNode;
        newNode->index = 0;
    }else {
        list->head = addElementRecursive(list->head,newNode);
    }
    list->size++;
    return INSERTION_OK;
}

node_t addElementRecursive(node_t node,node_t newNode) {
    if(node->next == null) {
        newNode->index = node->index + 1;
        return newNode;
    }
    node->next = addElementRecursive(node->next,newNode);
    return node;
}

int getElementOnIndex(listObject_t list,void *buffer,const unsigned int index) {
    node_t *aux;
    if(list == NULL) return NULL;
    if(list->head == NULL) return EMPTY_LIST_ERROR;
    aux = getElementOnIndexRecursive(list->head,index);
    if(aux == NULL) return ELEMENT_DOESNT_EXIST;
    memcpy(buffer,aux->element,aux->size);      //EVANS
	return aux->size;
}

node_t *getElementOnIndexRecursive(node_t node,const unsigned int index) {
    if(node->index == index) return node;
    if((node->next == NULL) || (index < node->index) )  return NULL;
    return getElementOnIndexRecursive(node->next,index);
}

int getFirstElement(listObject_t list,void * buffer) {
    if(list == NULL) return NULL_LIST_ERROR;
    if(list->head == NULL) return EMPTY_LIST_ERROR;
	memcpy(buffer,list->head->element,list->element->size);
	return list->element->size;
}

int removeElementOnIndex(listObject_t list,const unsigned int index) {
	int remotionState;
	if(list == NULL) return NULL_LIST_ERROR;
    if(list->head == NULL) return EMPTY_LIST_ERROR;
	list->head = removeElementOnIndexRecursive(list->head,index,&remotionState);
	return remotionState;
}

node_t *removeElementOnIndexRecursive(node_t node,const unsigned int index,int *remotionState) {
	node_t *aux;
	if(node->index == index) {
		free(node->element);	//EVANS
		aux = node->next;
		free(aux);				//EVANS
		*remotionState = REMOTION_OK;
		return aux;
	}
	if((node->next == NULL) || (index < node->index)) {
		*remotionState = ELEMENT_DOESNT_EXIST;
		return node;
	}
	return removeElementOnIndexRecursive(node->next,index,&remotionState);
}

int removeFirst(listObject_t list) {
	node_t *aux;
	if(list == NULL) return NULL_LIST_ERROR;
	if(list->head) return EMPTY_LIST_ERROR;
	aux = list->head->next;
	free(list->head->element);
	free(list->head);
	list->head = aux;
	return REMOTION_OK;
}

int size(listObject_t list) {
	if(list == NULL) return NULL_LIST_ERROR;
	return list->size;
}
