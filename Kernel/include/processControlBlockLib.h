#ifndef _PROCESS_CONTROL_BLOCK_LIB_H_

#define _PROCESS_CONTROL_BLOCK_LIB_H_

typedef struct processControlBlockList * processControlBlockListPtr_t;

#include <processControlBlock.h>
#include <memoryAllocator.h>

processControlBlockListPtr_t initializePCBList(void);
void addPCBToList(processControlBlockListPtr_t list, processControlBlockPtr_t pcb);
processControlBlockPtr_t removeFirstPCBFromList(processControlBlockListPtr_t list);
processControlBlockPtr_t consultFirstPCBFromList(processControlBlockListPtr_t list);
processControlBlockPtr_t PCBFromListByPID(processControlBlockListPtr_t list,
                                          long int pid);
processControlBlockListPtr_t concatenatePCBList(processControlBlockListPtr_t start,
                                                processControlBlockListPtr_t end);
void printListPid(processControlBlockListPtr_t list);

#endif
