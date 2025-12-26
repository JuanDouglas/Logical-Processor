#ifndef CLOCK_H
#define CLOCK_H

#include <stdbool.h>

/*
 * Clock global do processador.
 * Permite pause, step e inspeção.
 */
void clock_init(void);
void clock_tick(void);

bool clock_rising_edge(void);
int  clock_cycle(void);

void clock_pause(bool state);

#endif
