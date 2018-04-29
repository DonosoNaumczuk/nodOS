typedef struct ProcessControlBlockNode {
    ProcessControlBlockNode *next;
    ProcessControlBlock *value;
} ProcessControlBlockNode;

typedef struct ProcessControlBlockList {
    ProcessControlBlockNode *first;
    ProcessControlBlockNode *last;
    int quantity;
} ProcessControlBlockList;

ProcessControlBlockList *initializePCBList(void);
void addPCBToList(ProcessControlBlockList *list, ProcessControlBlock *pcb);
ProcessControlBlock *removeFirstPCBFromList(ProcessControlBlockList *list);
ProcessControlBlock *consultFirstPCBFromList(ProcessControlBlockList *list);
ProcessControlBlock *removeTerminatePCBFromListByPID(
                     ProcessControlBlockList *list, long int pid,
                     int *isInTheList);
ProcessControlBlock *removeNextPCBNode(ProcessControlBlockNode *current);
void concatenatePCBList(ProcessControlBlockList *start,
                        ProcessControlBlockList *end);
