#include <listADT.h>

typedef struct nodeStruct_t *node_t;

typedef struct nodeStruct_t{
    unsigned int index;
    unsigned int size;
    void *element;
    node_t next;
}nodeStruct_t;

typedef struct list_t{
    node_t head;
    int size;
}list_t;

void addElementRecursive(node_t node,node_t newNode);
node_t getElementOnIndexRecursive(node_t node,const unsigned int index);
node_t removeElementOnIndexRecursive(node_t node,const unsigned int index,int *remotionState);
int containsRecursive(node_t node,int (*compareTo)(void*,void*),void *element);
int getFirstElementByCriteriaRecursive(node_t node,int (*compareTo)(void*,void*),void *reference,void *buffer);

listObject_t newList() {
	listObject_t list = allocateMemory(sizeof(struct list_t));
	list->size = 0;
	return list;
}

int addElement(listObject_t list,void *element,const unsigned int size) {
    if(list == NULL) return NULL_LIST_ERROR;
    if(element == NULL) return NULL_ELEMENT_ERROR;
    if(size == 0)  return SIZE_ERROR;
    node_t newNode = allocateMemory(sizeof(node_t));
    newNode->element = allocateMemory(size);
    newNode->size = size;
    memcpy(newNode->element,element,size);
    if(list->head == NULL) {
        list->head = newNode;
        newNode->index = 0;
    }else {
        addElementRecursive(list->head,newNode);
    }
    list->size++;
    return INSERTION_OK;
}

void addElementRecursive(node_t node,node_t newNode) {
    if(node->next == NULL) {
        newNode->index = node->index + 1;
        node->next = newNode;
		return;
    }
    addElementRecursive(node->next,newNode);
    return;
}

int getElementOnIndex(listObject_t list,void *buffer,const unsigned int index) {
    node_t aux;
    if(list == NULL) return NULL_LIST_ERROR;
    if(list->head == NULL) return EMPTY_LIST_ERROR;
    aux = getElementOnIndexRecursive(list->head,index);
    if(aux == NULL) return ELEMENT_DOESNT_EXIST;
    memcpy(buffer,aux->element,aux->size);
	return aux->size;
}

node_t getElementOnIndexRecursive(node_t node,const unsigned int index) {
    if(node->index == index) return node;
    if((node->next == NULL) || (index < node->index) )  return NULL;
    return getElementOnIndexRecursive(node->next,index);
}

int getFirstElement(listObject_t list,void * buffer) {
    if(list == NULL) return NULL_LIST_ERROR;
    if(list->head == NULL) return EMPTY_LIST_ERROR;
	memcpy(buffer,list->head->element,list->head->size);
	return list->head->size;
}

int removeElementOnIndex(listObject_t list,const unsigned int index) {
	int remotionState;
	if(list == NULL) return NULL_LIST_ERROR;
    if(list->head == NULL) return EMPTY_LIST_ERROR;
	list->head = removeElementOnIndexRecursive(list->head,index,&remotionState);
	if(remotionState == REMOTION_OK) list->size--;
	return remotionState;
}

node_t removeElementOnIndexRecursive(node_t node,const unsigned int index,int *remotionState) {
	node_t aux;
	if(node->index == index) {
		aux = node->next;
		freeMemory(node->element);
		freeMemory(node);
		*remotionState = REMOTION_OK;
		return aux;
	}
	if((node->next == NULL) || (index < node->index)) {
		*remotionState = ELEMENT_DOESNT_EXIST;
		return node;
	}
	return removeElementOnIndexRecursive(node->next,index,remotionState);
}

int removeFirst(listObject_t list) {
	node_t aux;
	if(list == NULL) return NULL_LIST_ERROR;
	if(list->head == NULL) return EMPTY_LIST_ERROR;
	aux = list->head;
	list->head = list->head->next;
	freeMemory(aux->element);
	freeMemory(aux);
	list->size--;
	return REMOTION_OK;
}

int size(listObject_t list) {
	if(list == NULL) return NULL_LIST_ERROR;
	return list->size;
}

int contains(listObject_t list,int (*compareTo)(void*,void*),void *element){
	if(list == NULL) return NULL_LIST_ERROR;
	if(compareTo == NULL) return NULL_FUNCTION_POINTER;
	return containsRecursive(list->head,compareTo,element);
}

int containsRecursive(node_t node,int (*compareTo)(void*,void*),void *element) {
	if(node == NULL) return FALSE;
	if((*compareTo)(element,node->element) == 0) return TRUE;
	return containsRecursive(node->next,compareTo,element);
}

int getFirstElementByCriteria(listObject_t list,int (*compareTo)(void*,void*),void *reference,void *buffer) {
	if(list == NULL) return NULL_LIST_ERROR;
	if(compareTo == NULL) return NULL_FUNCTION_POINTER;
	return getFirstElementByCriteriaRecursive(list->head,compareTo,reference,buffer);
}

int getFirstElementByCriteriaRecursive(node_t node,int (*compareTo)(void*,void*),void *reference,void *buffer) {
	if(node == NULL) return ELEMENT_DOESNT_EXIST;
	if((*compareTo)(reference,node->element) == 0) {
		memcpy(buffer,node->element,node->size);
		return node->size;
	}
	return getFirstElementByCriteriaRecursive(node->next,compareTo,reference,buffer);
}
