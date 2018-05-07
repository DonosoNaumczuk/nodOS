#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>
#include <RTCReader.h>
#include <videoDriver.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
uint64_t timeHandler(int c);

#endif
