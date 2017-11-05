#ifndef _portIO_H_
#define _portIO_H_
#include <stdint.h>

uint16_t read_port(uint16_t port);
void write_port(uint16_t port, uint16_t data);
#endif
