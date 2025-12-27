#include "unit.h"
#include <stdio.h>
#include <string.h>

void unit_init(Unit *u, const char *name) {
    memset(u, 0, sizeof(Unit));
    strcpy(u->name, name);
}

void unit_add_component(Unit *u, Circuit c) {
    if (u->sub_count < MAX_SUBCIRCUITS) {
        u->subs[u->sub_count++] = c;
    }
}

void unit_add_wire(Unit *u, int src_idx, char src_pin, int dst_idx, char dst_pin) {
    if (u->wire_count < MAX_WIRES) {
        u->wires[u->wire_count++] = (Wire){src_idx, src_pin, dst_idx, dst_pin};
    }
}

void unit_map_input(Unit *u, char unit_pin, int dst_idx, char dst_pin) {
    unit_add_wire(u, -1, unit_pin, dst_idx, dst_pin);
}

void unit_map_output(Unit *u, char unit_pin, int src_idx, char src_pin) {
    unit_add_wire(u, src_idx, src_pin, -1, unit_pin);
}

void unit_set_pin(Unit *u, char pin, bool val) {
    // Injeta valor diretamente no componente destino
    for (int i = 0; i < u->wire_count; i++) {
        Wire *w = &u->wires[i];
        if (w->src_idx == -1 && w->src_pin == pin) {
            circuit_set(&u->subs[w->dst_idx], w->dst_pin, val); //
        }
    }
}

bool unit_get_pin(Unit *u, char pin) {
    // Lê valor diretamente do componente fonte
    for (int i = 0; i < u->wire_count; i++) {
        Wire *w = &u->wires[i];
        if (w->dst_idx == -1 && w->dst_pin == pin) {
            return circuit_get(&u->subs[w->src_idx], w->src_pin);
        }
    }
    return false;
}

/*
 * O CORE DA SIMULAÇÃO
 * Ordem: Eval -> Commit -> Route -> Clock
 */
void unit_step(Unit *u) {
    // 1. Lógica Combinacional (Calcula Next State)
    for (int i = 0; i < u->sub_count; i++) {
        circuit_eval(&u->subs[i]);
    }

    // 2. Borda de Clock (Next -> Value)
    for (int i = 0; i < u->sub_count; i++) {
        circuit_commit(&u->subs[i]);
    }

    // 3. Roteamento (Propagação elétrica pelos fios)
    for (int i = 0; i < u->wire_count; i++) {
        Wire *w = &u->wires[i];

        // Ignora entradas externas (já tratadas pelo unit_set_pin)
        // Ignora saídas externas (serão lidas pelo unit_get_pin)
        if (w->src_idx == -1 || w->dst_idx == -1) continue;

        // Copia sinal: Fonte -> Destino
        bool val = circuit_get(&u->subs[w->src_idx], w->src_pin);
        circuit_set(&u->subs[w->dst_idx], w->dst_pin, val);
    }

    clock_tick();
}

void unit_run(Unit *u, int cycles) {
    for(int i=0; i<cycles; i++) unit_step(u);
}

/* refs/base/unit.c */

#include <stdio.h>
#include "unit.h" // Certifique-se de que clock.h está incluso aqui ou no unit.h

void unit_dump(Unit *u) {
    printf("\n=== DIAGNOSTIC DUMP: %s [Cycle %d] ===\n", u->name, clock_cycle());

    // 1. ESTADO DOS COMPONENTES (Portas/Registradores)
    printf("[COMPONENTS]\n");
    for (int i = 0; i < u->sub_count; i++) {
        Circuit *c = &u->subs[i];

        // Exibe Índice e Tipo (ex: #0 ALU_SLICE)
        // Nota: A ordem #0, #1 segue a ordem de declaração USE no cpu.txt
        printf("  #%02d %-12s : ", i, c->name);

        // Exibe todos os sinais internos (Entradas, Saídas, Internos)
        for(int j=0; j < c->signal_count; j++) {
            Signal *s = &c->signals[j];
            // Mostra apenas se o valor for 1 para facilitar leitura (ou 0/1 se preferir)
            printf("%c=%d ", s->name, s->value);
        }
        printf("\n");
    }

    // 2. ESTADO DOS FIOS (Conexões Ativas)
    printf("[ACTIVE WIRES]\n");
    bool header_printed = false;
    for (int i = 0; i < u->wire_count; i++) {
        Wire *w = &u->wires[i];

        // Ignora conexões externas no dump detalhado para reduzir ruído
        if (w->src_idx != -1 && w->dst_idx != -1) {
            bool val = circuit_get(&u->subs[w->src_idx], w->src_pin);

            // Destaque visual: Mostra fluxo A -> B
            printf("  Wire %02d: Comp#%d.%c (%d) ---> Comp#%d.%c\n",
                   i,
                   w->src_idx, w->src_pin, val,
                   w->dst_idx, w->dst_pin);
        }
    }
    printf("==========================================\n");
}