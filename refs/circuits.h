#ifndef CIRCUITS_H
#define CIRCUITS_H

#include "base/circuit.h"

/* Portas básicas */
Circuit circuit_and(void);
Circuit circuit_or(void);
Circuit circuit_not(void);
Circuit circuit_nand(void);
Circuit circuit_nor(void);
Circuit circuit_xor(void);
Circuit circuit_xnor(void);

/* Circuitos combinacionais */
Circuit circuit_mux2(void);    /* A, B, S -> Y */
Circuit circuit_demux2(void);  /* A, S -> Y0, Y1 */
Circuit circuit_full_adder(void);
Circuit circuit_half_adder(void);
Circuit circuit_register_1bit(void);
#endif
