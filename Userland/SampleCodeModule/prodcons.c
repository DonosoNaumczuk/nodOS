MAX_PRODCONS#define QUIT 	 		 'q'
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
	char *ciriticalZone;
	int *index;
} prodcons_t;

void prodcons() {
	printMenu();
	createMutualExclusion(MUTEX_CONS);
	createMutualExclusion(MUTEX_PROD);
	createSemaphore(SEM_FULL, BUFFER_SIZE);
	createSemaphore(SEM_EMPTY, 0);
	char criticalZone[BUFFER_SIZE + 1];
	initCriticalZone(criticalZone);
	int index = 0;

	uint64_t consumers[MAX_PRODCONS] = {0};
	int consumersQty = 0;
	int producers[MAX_PRODCONS] = {0};
	uint64_t producersQty = 0;

	prodcons_t consumerStruct;
	consumerStruct.list = consumers;
	consumerStruct.size = &consumersQty;
	consumerStruct.criticalZone = criticalZone;
	consumerStruct.index = &index;

	prodcons_t producerStruct;
	producerStruct.list = producers;
	producerStruct.size = &producersQty;
	producerStruct.criticalZone = criticalZone;
	producerStruct.index = &index;

	int c;
	while((c = getchar()) != QUIT) {
		switch(c) {
			case INC_PROD:
				incrementProducer(producers, &producersQty);
				break;
			case DEC_PROD:
				decrementProducer(producers, &producersQty);
				break;
			case INC_CONS:
				incrementConsumer(consumers, &consumersQty);
				break;
			case DEC_CONS:
				decrementConsumer(consumers, &consumersQty);
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

	void * arguments[4];
	int mode = FOREGROUND;
	int *id = allocateMemory()
	*id = getNullId(producerStruct);
	arguments[0] = &mode;
	arguments[1] = "producer";
	arguments[2] = producerStruct;
	arguments[3] = id;
	consumerStruct->list[*id] = createProcess(&consumer, 4, arguments);
	*consumerStruct->size++;
	unlock(MUTEX_PROD);
}

void decrementProducer(prodcons_t producerStruct) {
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

	void * arguments[4];
	int mode = FOREGROUND;
	int id = getNullId(consumerStruct);
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

int getNotNullId(prodcons_t prodcons) {
	for(int i = 0; i < MAX_PRODCONS; i++) {
		if(prodcons->list[i] != NULL) {
			return i;
		}
	}

	return -1; /* can't happen */
}

int getNullId(prodcons_t prodcons) {
	int getNotNullId(prodcons_t prodcons) {
		for(int i = 0; i < MAX_PRODCONS; i++) {
			if(prodcons->list[i] == NULL) {
				return i;
			}
		}

		return -1; /* can't happen */
	}
}

int producer(int argc, void ** args) {
	semaphoreWait(SEM_FULL);
	lock(MUTEX_PROD);
	lock(MUTEX_CONS);

	prodcons_t *producerStruct = args[0];
	int id = *args[1];

	/* Write on critical zone my id */
	int index = *producerStruct->index++;
	producerStruct->criticalZone[index] = id + '0';

	/* Terminate */
	producerStruct->list[id] = NULL;
	*producerStruct->size--;

	unlock(MUTEX_CONS);
	unlock(MUTEX_PROD);
	semaphorePost(SEM_EMPTY);
	return 0;
}

int consumer(int argc, void ** args) {
	semaphoreWait(SEM_EMPTY);
	lock(MUTEX_PROD);
	lock(MUTEX_CONS);

	/* Read on critical zone writing it as empty */
	int index = *--consumerStruct->index;
	consumerStruct->criticalZone[index] = EMPTY_SPACE;

	/* Terminate */
	consumerStruct->list[id] = NULL;
	*consumerStruct->size--;

	unlock(MUTEX_CONS);
	unlock(MUTEX_PROD);
	semaphoreWait(SEM_EMPTY);
}

void printMenu() {
	printf("You can create at least %d consumers and producer\n", MAX_PRODCONS);
	printf("Press:\n", );
	putchar(INC_PROD);
	printf(": Increment producers\n");
	putchar(DEC_PROD);
	printf(": Decrement producers\n");
	putchar(INC_CONS);
	printf(": Increment consumers\n");
	putchar(DEC_CONS);
	printf(": Decrement consumers\n";
	putchar(QUIT);
	printf(": Quit\n");
}
