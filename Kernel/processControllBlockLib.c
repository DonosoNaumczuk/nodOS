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
    aux->quantity = 0;

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

processControlBlockPtr_t PCBFromListByPID(processControlBlockListPtr_t list,
                                          long int pid) {
    processControlBlockPtr_t output = NULL;
    if(list != NULL && list->first != NULL) {
        if(isThisPid(list->first->value, pid)) {
            output = removeFirstPCBFromList(list);
        }
        processControlBlockNode *current = list->first;
        while(current->next != NULL) {
            if(isThisPid(current->next->value, pid)) {
                output = removeNextPCBNode(current);
                list->quantity--;
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

processControlBlockListPtr_t concatenatePCBList(processControlBlockListPtr_t start,
                                                processControlBlockListPtr_t end) {
    processControlBlockListPtr_t aux;
    if(start != NULL) {
        aux = start;
        if(end != NULL && end->quantity != 0) {
            if(start->quantity == 0) {
                start->first = end->first;
            }
            else {
                start->last->next = end->first;
            }
            start->last = end->last;
            start->quantity += end->quantity;
        }
    } else {
        aux = end;
    }
    return aux;
}
