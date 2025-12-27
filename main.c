#include <stdio.h>
#include <stdlib.h>
#include "refs/loader.h"
#include "refs/base/clock.h"

// Função de pulso seguro
void pulse_clock(Unit *u) {
    printf("   [CLK] Propagating...\n");
    for(int i=0; i<8; i++) unit_step(u);

    // Como mapeamos IN.W para rN.E (Enable), o "Clock" aqui é pulsar o W
    // Em um design real, o Clock seria um pino separado, mas usamos Enable como clock
    unit_set_pin(u, 'W', 1);
    unit_step(u);
    unit_set_pin(u, 'W', 0);
    unit_step(u);
}

int main(void) {
    clock_init();
    Unit cpu;

    printf("=== DYNAMIC CPU LOADER ===\n");
    // Carrega a arquitetura inteira do arquivo texto!
    if (!loader_load_from_file("cpu.txt", &cpu, "CPU_4BIT")) {
        printf("Falha ao carregar cpu.txt\n");
        return 1;
    }
    printf("Arquitetura '%s' carregada com sucesso!\n", cpu.name);

    // SETUP INICIAL (Reset)
    unit_set_pin(&cpu, 'L', 1); // ADD
    unit_set_pin(&cpu, 'H', 1); // ADD
    unit_set_pin(&cpu, 'I', 0); // Cin=0
    unit_set_pin(&cpu, 'W', 0); // Write=0

    // B = 1 (Incremento)
    unit_set_pin(&cpu, 'a', 1); // No arquivo cpu.txt, mapeei IN.a -> u0.B (entrada B)
    unit_set_pin(&cpu, 'b', 0);
    unit_set_pin(&cpu, 'c', 0);
    unit_set_pin(&cpu, 'd', 0);

    printf("Comandos: [w]=Clock Pulse, [d]=Dump State, [r]=Reload File, [q]=Quit\n");
    char cmd[16];

    while(1) {
        // Monitor simples de saída
        int val = 0;
        for(int i=0; i<4; i++) if(unit_get_pin(&cpu, '0'+i)) val |= (1<<i);

        printf("[Cycle %02d] OUT=%d > ", clock_cycle(), val);
        fgets(cmd, sizeof(cmd), stdin);

        // === COMANDOS ===

        if (cmd[0] == 'q') break;

        // COMANDO DE DUMP
        else if (cmd[0] == 'd') {
            unit_dump(&cpu); // <--- Chama a função de diagnóstico
        }

        else if (cmd[0] == 'w') {
            pulse_clock(&cpu);
        }

        else if (cmd[0] == 'r') {
            printf("Reloading cpu.txt...\n");
            // Nota: Em um cenário real, você deveria dar um free() na cpu antiga antes
            if(loader_load_from_file("cpu.txt", &cpu, "CPU_4BIT"))
                printf("Success!\n");
            else
                printf("Failed.\n");
        }
        else {
            unit_step(&cpu); // Step manual (tick)
        }
    }

    return 0;
}