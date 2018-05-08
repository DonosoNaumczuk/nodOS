#include <messageQueue.h>
#include <videoDriver.h>

typedef struct mailboxStruct_t *mailbox_t;

typedef struct mailboxStruct_t{
	char *mailboxId;
	listObject_t messageQueue;
}mailboxStruct_t;

static listObject_t mailboxList;

static mailbox_t newMailbox(const char *mailboxId);
static int existMailbox(char *mailboxId,mailbox_t mailbox);

void initMessageQueue() {
	 mailboxList = newList();
	 createMutualExclusion(MUTEX_NAME,getProcessID());
}

int createMailbox(const char *mailboxId) {
	lock(MUTEX_NAME,getProcessID());
	if(!contains(mailboxList,&existMailbox,mailboxId)) addElement(mailboxList,newMailbox(mailboxId),sizeof(mailboxStruct_t));
	unlock(MUTEX_NAME,getProcessID());
}

void send(const char *mailboxId,const void *message,const unsigned int messageSize) {
	lock(stringConcatenation(MUTEX_NAME,mailboxId),getProcessID());
	mailbox_t mailbox = getFirstElementReferenceByCriteria(mailboxList,&existMailbox,mailboxId);
	addElement(mailbox->messageQueue,message,messageSize);
	semaphorePost(stringConcatenation(SEMAPHORE_NAME,mailboxId),getProcessID());
	unlock(stringConcatenation(MUTEX_NAME,mailboxId),getProcessID());
}

void *receive(const char *mailboxId) {
	semaphoreWait(stringConcatenation(SEMAPHORE_NAME,mailboxId),getProcessID());
	lock(stringConcatenation(MUTEX_NAME,mailboxId),getProcessID());

	mailbox_t mailbox = getFirstElementReferenceByCriteria(mailboxList,&existMailbox,mailboxId);
	void *reference = getFirstElementReferece(mailbox->messageQueue);
	removeFirst(mailbox->messageQueue);

	unlock(stringConcatenation(MUTEX_NAME,mailboxId),getProcessID());
	return reference;
}

void closeMailbox(const char *mailboxId) {
	lock(MUTEX_NAME,getProcessID());
	lock(stringConcatenation(MUTEX_NAME,mailboxId),getProcessID());

	mailbox_t mailbox = getFirstElementReferenceByCriteria(mailboxList,&existMailbox,mailboxId);
	removeAndFreeAllElements(mailbox->messageQueue);
	removeAndFreeFirstElementByCriteria(mailboxList,&existMailbox,mailboxId);

	unlock(stringConcatenation(MUTEX_NAME,mailboxId),getProcessID());
	//mutexClose(stringConcatenation(MUTEX_NAME,mailboxId),getProcessID());
	unlock(MUTEX_NAME,getProcessID());
}

//NECESITO STRING COPY Y STRING LENGTH
static mailbox_t newMailbox(const char *mailboxId) {
	mailbox_t newMailbox = allocateMemory(sizeof(mailboxStruct_t));
	newMailbox->mailboxId = allocateMemory(stringLength(mailboxId) + 1);
	stringCopy(newMailbox->mailboxId,mailboxId);
	newMailbox->messageQueue = newList();
	createSemaphore(stringConcatenation(SEMAPHORE_NAME,mailboxId),0,getProcessID());
	createMutualExclusion(stringConcatenation(MUTEX_NAME,mailboxId),getProcessID());
	return newMailbox;
}

static int existMailbox(char *mailboxId,mailbox_t mailbox) {
	return	stringCompare(mailboxId,mailbox->mailboxId);
}
