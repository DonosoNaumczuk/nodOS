#include <shell.h>
#include <syscall.h>

int main() {
     uint64_t processId;
     processId = createProcess(&shell, 0, NULL);
     waitChild(processId);
     printf("ya volvio.\n");
     return 0;
}
