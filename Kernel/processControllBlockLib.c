#include <processControlBlockLib.h>

typedef struct processControlBlockNode {
    struct processControlBlockNode *next;
    processControlBlockPtr_t value;
} processControlBlockNode;

typedef struct processControlBlockList {
    processControlBlockNode *first;
    processControlBlockNode *last;
    int quantity;
} processControlBlockList;

static processControlBlockPtr_t removeNextPCBNode(processControlBlockNode *current);

processControlBlockListPtr_t initializePCBList() {
   processControlBlockListPtr_t  aux = allocateMemory(sizeof(processControlBlockList));
    aux->first = NULL;
    aux->last = NULL;
    aux->quantity = NULL;
    return aux;
}

void addPCBToList(processControlBlockListPtr_t list, processControlBlockPtr_t pcb) {
    if(list != NULL && pcb != NULL) {
        processControlBlockNode *pcbNode = allocateMemory(sizeof(processControlBlockNode));
        pcbNode->next = NULL;
        pcbNode->value = pcb;
        if(list->quantity == 0) {
            list->first = pcbNode;
        }
        else {
            list->last->next = pcbNode;
        }
        list->last = pcbNode;
        list->quantity++;
    }
}

processControlBlockPtr_t removeFirstPCBFromList(processControlBlockListPtr_t list) {
    processControlBlockPtr_t aux = NULL;
    if (list != NULL && list->quantity != 0) {
        aux = list->first->value;
        processControlBlockNode *auxFirst = list->first;
        if (list->quantity == 1) {
            list->first = NULL;
            list->last = NULL;
        }
        else {
            list->first = list->first->next;
        }
        list->quantity--;
        freeMemory(auxFirst);
    }
    return aux;
}

processControlBlockPtr_t consultFirstPCBFromList(processControlBlockListPtr_t list) {
    return (list != NULL && list->first != NULL) ? list->first->value : NULL;
}

/* returns the pcb if the process is terminate and in the ProcessList and null
 * otherwise
 * isInTheList is an output parameter that indicates if the process is in the
 * list
 */
processControlBlockPtr_t removeTerminatePCBFromListByPID(
                        processControlBlockListPtr_t list, long int pid,
                         int *isInTheList) {
    processControlBlockPtr_t output = NULL;
    *isInTheList = FALSE;
    if(list != NULL && list->first != NULL) {
        if(isTerminateAndTheSameProcess(list->first->value, pid)) {
            output = removeFirstPCBFromList(list);
            *isInTheList = TRUE;
        }
        processControlBlockNode *current = list->first;
        while(current->next != NULL && !(*isInTheList)) {
            if(isTerminateAndTheSameProcess(current->next->value, pid)) {
                if(current->next == list->last) {
                    list->last = current;
                }
                output = removeNextPCBNode(current);
                *isInTheList = TRUE;
            }
        }
    }
    return output;
}

processControlBlockPtr_t removeNextPCBNode(processControlBlockNode *current) {
    processControlBlockNode *toBeFree = current->next;
    processControlBlockPtr_t output = current->next->value;
    current->next = current->next->next;
    freeMemory(toBeFree);
    return output;
}

void concatenatePCBList(processControlBlockListPtr_t start,
                       processControlBlockListPtr_t end) {
    if(start != NULL && end != NULL && end->quantity != 0) {
        if(start->quantity == 0) {
            start->first = end->first;
        }
        else {
            start->last->next = end->first;
        }
        start->last = end->last;
        start->quantity += end->quantity;
    }
}
