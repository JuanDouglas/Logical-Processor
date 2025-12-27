#ifndef UNIT_H
#define UNIT_H

#include "base/circuit.h"
#include "base/clock.h"

#define MAX_SUBCIRCUITS 16
#define MAX_WIRES 64

/*
 * Estrutura de Conexão.
 * src_idx = -1 indica Entrada da Unidade (Unit Input Pin).
 * dst_idx = -1 indica Saída da Unidade (Unit Output Pin).
 */
typedef struct {
    int src_idx;
    char src_pin;
    int dst_idx;
    char dst_pin;
} Wire;

typedef struct {
    char name[32];
    Circuit subs[MAX_SUBCIRCUITS];
    int sub_count;
    Wire wires[MAX_WIRES];
    int wire_count;
} Unit;

/* --- Construção --- */
void unit_init(Unit *u, const char *name);
void unit_add_component(Unit *u, Circuit c);

/* Roteamento Interno e Mapeamento de Pinos */
void unit_add_wire(Unit *u, int src_idx, char src_pin, int dst_idx, char dst_pin);
void unit_map_input(Unit *u, char unit_pin, int dst_idx, char dst_pin);
void unit_map_output(Unit *u, char unit_pin, int src_idx, char src_pin);

/* --- Operação --- */
// Define valor em um pino de entrada da Unidade
void unit_set_pin(Unit *u, char pin, bool val);
// Lê valor de um pino de saída da Unidade
bool unit_get_pin(Unit *u, char pin);

/* --- Simulação e Debug --- */
// Avança 1 Ciclo (Eval -> Commit -> Route)
void unit_step(Unit *u);
// Avança N Ciclos
void unit_run(Unit *u, int cycles);
// Exibe estado interno completo
void unit_dump(Unit *u);

#endif