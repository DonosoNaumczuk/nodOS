ProcessControlBlockList *initializePCBList() {
    return calloc(1, sizeof(ProcessControlBlockList))
}

void addPCBToList(ProcessControlBlockList *list, ProcessControlBlock *pcb) {
    if(list != NULL && pcb != NULL) {
        ProcessControlBlockNode *pcbNode = calloc(1,
                                           sizeof(ProcessControlBlockNode));
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

ProcessControlBlock *removeFirstPCBFromList(ProcessControlBlockList *list) {
    ProcessControlBlock *aux = NULL;
    if (list != NULL && list->quantity != 0) {
        aux = list->first->value;
        ProcessControlBlockNode *auxFirst = list->first;
        if (list->quantity == 1) {
            list->first = NULL;
            list->last = NULL;
        }
        else {
            list->first = list->first->next;
        }
        list->quantity--;
        free(auxFirst);
    }
    return aux;
}

ProcessControlBlock *consultFirstPCBFromList(ProcessControlBlockList *list) {
    return (list != NULL && list->first != NULL) ? list->first->value : NULL;
}

/* returns the pcb if the process is terminate and in the ProcessList and null
 * otherwise
 * isInTheList is an output parameter that indicates if the process is in the
 * list
 */
ProcessControlBlock *removeTerminatePCBFromListByPID(
                     ProcessControlBlockList *list, long int pid,
                     int *isInTheList) {
    ProcessControlBlock *output = NULL;
    *isInTheList = FALSE;
    if(list != NULL && list->first != NULL) {
        if(isTerminateAndTheSameProcess(list->first->value, pid)) {
            output = removeFirstPCBFromList(list);
            *isInTheList = TRUE;
        }
        ProcessControlBlockNode *current = list->first;
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

ProcessControlBlock *removeNextPCBNode(ProcessControlBlockNode *current) {
    ProcessControlBlockNode *toBeFree = current->next;
    ProcessControlBlock *output = current->next->value;
    current->next = current->next->next;
    free(toBeFree);
    return output;
}

void concatenatePCBList(ProcessControlBlockList *start,
                        ProcessControlBlockList *end) {
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
