#ifndef PRODCONS_H

#define PRODCONS_H

#include <lib.h>
#include <syscall.h>

#define QUIT 	 		 'q'
#define INC_PROD 		 'p'
#define DEC_PROD 		 'o'
#define INC_CONS 		 'c'
#define DEC_CONS 		 'x'
#define MAX_PRODCONS 	 10
#define BUFFER_SIZE 	100
#define NULL 	 	 	  0
#define EMPTY_SPACE		 '-'
#define MUTEX_PROD		"producerMutex"
#define MUTEX_CONS		"consumerMutex"
#define SEM_FULL		"fullSemaphore"
#define SEM_EMPTY		"emptySemaphore"
#define FOREGROUND 		  1

typedef struct {
	uint64_t *list;
	int *size;
	char *criticalZone;
	int *index;
} prodcons_t;

void prodcons();

void printCriticalZone(char *criticalZone);

void initCriticalZone(char *criticalZone);

void incrementProducer(prodcons_t *producerStruct);

void decrementProducer(prodcons_t *producerStruct);

void incrementConsumer(prodcons_t *consumerStruct);

void decrementConsumer(prodcons_t *consumerStruct);

int getNotNullId(prodcons_t *prodcons);

int getNullId(prodcons_t *prodcons);

int producer(int argc, void ** args);

int consumer(int argc, void ** args);

void printMenu();

#endif
