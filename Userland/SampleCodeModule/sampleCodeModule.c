#include <syscall.h>

int main() {
	write("anda la systemcall",18,0x0F);
	while(1);
}
