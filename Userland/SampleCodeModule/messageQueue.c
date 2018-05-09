#include <messageQueue.h>

int createMailbox(const char *mailboxId) {
     return _int_80(CREATE_MAILBOX, mailboxId, 0, 0);
}

void send(const char *mailboxId, const void *message, const unsigned int messageSize) {
     _int_80(SEND_MAILBOX, mailboxId, message, messageSize);
}

void * receive(const char *mailboxId) {
     _int_80(RECEIVE_MAILBOX, mailboxId, 0, 0);
}

void closeMailbox(const char *mailboxId) {
     _int_80(DESTROY_MAILBOX, mailboxId, 0, 0);
}
