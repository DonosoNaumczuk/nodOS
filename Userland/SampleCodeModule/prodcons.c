#include <prodcons.h>

void prodcons() {
	printMenu();
	createMutualExclusion(MUTEX_CONS);
	createMutualExclusion(MUTEX_PROD);
	createSemaphore(SEM_FULL, BUFFER_SIZE);
	createSemaphore(SEM_EMPTY, 0);
	char criticalZone[BUFFER_SIZE + 1];
	initCriticalZone(criticalZone);
	int index = 0;

	printCriticalZone(criticalZone);

	uint64_t consumers[MAX_PRODCONS] = {0};
	int consumersQty = 0;
	int producers[MAX_PRODCONS] = {0};
	uint64_t producersQty = 0;

	prodcons_t *consumerStruct = allocateMemory(1);//evans
	consumerStruct->list = consumers;
	consumerStruct->size = &consumersQty;
	consumerStruct->criticalZone = criticalZone;
	consumerStruct->index = &index;

	prodcons_t *producerStruct = allocateMemory(1);//evans
	producerStruct->list = producers;
	producerStruct->size = &producersQty;
	producerStruct->criticalZone = criticalZone;
	producerStruct->index = &index;

	int c;
	while((c = getchar()) != QUIT) {
		switch(c) {
			case INC_PROD:
				incrementProducer(producerStruct);
				break;
			case DEC_PROD:
				decrementProducer(producerStruct);
				break;
			case INC_CONS:
				incrementConsumer(consumerStruct);
				break;
			case DEC_CONS:
				decrementConsumer(consumerStruct);
				break;
		}
	}
}

void printCriticalZone(char *criticalZone) {
	printf("Critical zone buffer\n");
	printf("The ' - ' character indicates empty\n\n");
	printf("[ ");
	printf(criticalZone);
	printf(" ]\n");
}

void initCriticalZone(char *criticalZone) {
	for(int i = 0; i < BUFFER_SIZE; i++) {
		criticalZone[i] = EMPTY_SPACE;
	}
	criticalZone[BUFFER_SIZE] = 0;
}

void incrementProducer(prodcons_t *producerStruct) {
	lock(MUTEX_PROD);
	int *size = producerStruct->size;
	if(*size >= MAX_PRODCONS) {
		printf("Produces already reach the max quantity\n");
		unlock(MUTEX_PROD);
		return;
	}

	void ** arguments = allocateMemory(sizeof(void*)*4);
	int mode = FOREGROUND;
	int *id = allocateMemory(sizeof(*id));
	*id = getNullId(producerStruct);
	arguments[0] = &mode;
	arguments[1] = "producer";
	arguments[2] = producerStruct;
	arguments[3] = id;
	producerStruct->list[*id] = createProcess(&producer, 4, arguments);
	*producerStruct->size++;
	unlock(MUTEX_PROD);
}

void decrementProducer(prodcons_t *producerStruct) {
	lock(MUTEX_PROD);
	int *size = producerStruct->size;
	if(*size == 0) {
		printf("Producers already reach the min quantity\n");
		unlock(MUTEX_PROD);
		return;
	}

	terminateProcess(producerStruct->list[getNotNullId(producerStruct)]);
	*producerStruct->size--;

	unlock(MUTEX_PROD);
}

void incrementConsumer(prodcons_t *consumerStruct) {
	lock(MUTEX_CONS);
	int *size = consumerStruct->size;
	if(*size >= MAX_PRODCONS) {
		printf("Consumers already reach the max quantity\n");
		unlock(MUTEX_CONS);
		return;
	}

	void ** arguments = allocateMemory(sizeof(void*)*4);;
	int mode = FOREGROUND;
	int *id = allocateMemory(sizeof(*id));
	*id = getNullId(consumerStruct);
	arguments[0] = &mode;
	arguments[1] = "consumer";
	arguments[2] = consumerStruct;
	arguments[3] = &id;
	consumerStruct->list[*id] = createProcess(&consumer, 4, arguments);
	*consumerStruct->size++;
	unlock(MUTEX_CONS);
}

void decrementConsumer(prodcons_t *consumerStruct) {
	lock(MUTEX_CONS);
	int *size = consumerStruct->size;
	if(*size == 0) {
		printf("Consumers already reach the min quantity\n");
		unlock(MUTEX_CONS);
		return;
	}

	terminateProcess(consumerStruct->list[getNotNullId(consumerStruct)]);
	*consumerStruct->size--;

	unlock(MUTEX_CONS);
}

int getNotNullId(prodcons_t *prodcons) {
	for(int i = 0; i < MAX_PRODCONS; i++) {
		if(prodcons->list[i] != NULL) {
			return i;
		}
	}

	return -1; /* can't happen */
}

int getNullId(prodcons_t *prodcons) {
	for(int i = 0; i < MAX_PRODCONS; i++) {
		if(prodcons->list[i] == NULL) {
			return i;
		}
	}

	return -1; /* can't happen */
}

int producer(int argc, void ** args) {
	semaphoreWait(SEM_FULL);
	lock(MUTEX_PROD);
	lock(MUTEX_CONS);

	prodcons_t *producerStruct = args[0];
	int id = *(int *)args[1];

	/* Write on critical zone my id */
	int index = *producerStruct->index;
	*producerStruct->index = index + 1;

	producerStruct->criticalZone[index] = id + '0';

	/* Terminate */
	producerStruct->list[id] = NULL;
	*producerStruct->size--;

	printCriticalZone(producerStruct->criticalZone);

	unlock(MUTEX_CONS);
	unlock(MUTEX_PROD);
	semaphorePost(SEM_EMPTY);
	return 0;
}

int consumer(int argc, void ** args) {
	semaphoreWait(SEM_EMPTY);
	lock(MUTEX_PROD);
	lock(MUTEX_CONS);

	prodcons_t *consumerStruct = args[0];
	int id = *(int *)args[1];


	/* Read on critical zone writing it as empty */
	int index = *consumerStruct->index;
	index--;
	*consumerStruct->index = index;
	consumerStruct->criticalZone[index] = EMPTY_SPACE;

	/* Terminate */
	consumerStruct->list[id] = NULL;
	*consumerStruct->size--;

	printCriticalZone(consumerStruct->criticalZone);

	unlock(MUTEX_CONS);
	unlock(MUTEX_PROD);
	semaphorePost(SEM_FULL);
}

void printMenu() {
	printf("You can create at least %d consumers and producer\n", MAX_PRODCONS);
	printf("Press:\n");
	putChar(INC_PROD);
	printf(": Increment producers\n");
	putChar(DEC_PROD);
	printf(": Decrement producers\n");
	putChar(INC_CONS);
	printf(": Increment consumers\n");
	putChar(DEC_CONS);
	printf(": Decrement consumers\n");
	putChar(QUIT);
	printf(": Quit\n");
}
