#ifndef QUADRATIC_GRAFICATOR_H
#define QUADRATIC_GRAFICATOR_H

#include <syscall.h>
#include <stdint.h>
#include <lib.h>

void graphInit();
void graph(int a, int b, int c);
void graphWithScale(int a, int b, int c, int escale_factor_x, int escale_factor_y);
void estrellita();
void printAxiX();
void printAxiY();

#endif