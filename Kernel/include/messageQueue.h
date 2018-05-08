#ifndef MESSAGE_QUEUE
#define MESSAGE_QUEUE

#define MUTEX_NAME "!__MessegeQueueSystemMutex__!"
#define SEMAPHORE_NAME "!__MessegeQueueSystemSemaphore__!"

#include <listADT.h>
#include <mutualExclusion.h>
#include <lib.h>
#include <semaphore.h>
#include <scheduler.h>

int createMailbox(const char *mailboxID);
void send(const char *mailboxId, const void *message, const unsigned int messageSize);
void * receive(const char *mailboxId);
void closeMailbox(const char *mailboxID);
void startMessageQueue();

#endif
