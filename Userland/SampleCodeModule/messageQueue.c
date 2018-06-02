#include <messageQueue.h>

int createMailbox(const char *mailboxId) {
     return _int_80(CREATE_MAILBOX, (uint64_t) mailboxId, 0, 0);
}

void send(const char *mailboxId, const void *message, const unsigned int messageSize) {
     _int_80(SEND_MAILBOX, (uint64_t) mailboxId, (uint64_t) message, messageSize);
}

void * receive(const char *mailboxId) {
    return (void *) _int_80(RECEIVE_MAILBOX, (uint64_t) mailboxId, 0, 0);
}

void closeMailbox(const char *mailboxId) {
     _int_80(DESTROY_MAILBOX, (uint64_t) mailboxId, 0, 0);
}
