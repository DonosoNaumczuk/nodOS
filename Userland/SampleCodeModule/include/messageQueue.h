#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <syscall.h>

int createMailbox(const char *mailboxId);
void send(const char *mailboxId, const void *message, const unsigned int messageSize);
void *receive(const char *mailboxId);
void closeMailbox(const char *mailboxId);

#endif
