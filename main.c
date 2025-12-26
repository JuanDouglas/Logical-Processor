#include <stdio.h>
#include <stdbool.h>

#include "circuits.h"
#include "base/circuit.h"

/* =========================
 * UTILIDADES
 * ========================= */

static void pause_console(void)
{
    printf("\nPressione ENTER para continuar...");
    getchar();
}

static void print_bool(bool v)
{
    printf("%d", v ? 1 : 0);
}

/* =========================
 * TESTES DE PORTAS BINÁRIAS
 * ========================= */

void test_binary_gate(const char *name, Circuit (*factory)(void))
{
    Circuit c = factory();

    printf("\n==============================\n");
    printf(" TESTE: %s\n", name);
    printf("==============================\n");

    circuit_dump(&c);

    printf("\nA B | Y\n");
    printf("---------\n");

    for (int a = 0; a <= 1; a++)
    {
        for (int b = 0; b <= 1; b++)
        {
            circuit_set(&c, 'A', a);
            circuit_set(&c, 'B', b);

            circuit_eval(&c);

            printf("%d %d | ", a, b);
            print_bool(circuit_get(&c, 'Y'));
            printf("\n");
        }
    }

    pause_console();
}

/* =========================
 * TESTE DE PORTA NOT
 * ========================= */

void test_not_gate(void)
{
    Circuit c = circuit_not();

    printf("\n==============================\n");
    printf(" TESTE: NOT\n");
    printf("==============================\n");

    circuit_dump(&c);

    printf("\nA | Y\n");
    printf("-----\n");

    for (int a = 0; a <= 1; a++)
    {
        circuit_set(&c, 'A', a);
        circuit_eval(&c);

        printf("%d | ", a);
        print_bool(circuit_get(&c, 'Y'));
        printf("\n");
    }

    pause_console();
}

/* =========================
 * TESTE MUX 2:1
 * ========================= */

void test_mux2(void)
{
    Circuit c = circuit_mux2();

    printf("\n==============================\n");
    printf(" TESTE: MUX 2:1\n");
    printf("==============================\n");

    circuit_dump(&c);

    printf("\nA B S | Y\n");
    printf("-----------\n");

    for (int a = 0; a <= 1; a++)
    {
        for (int b = 0; b <= 1; b++)
        {
            for (int s = 0; s <= 1; s++)
            {
                circuit_set(&c, 'A', a);
                circuit_set(&c, 'B', b);
                circuit_set(&c, 'S', s);

                circuit_eval(&c);

                printf("%d %d %d | ", a, b, s);
                print_bool(circuit_get(&c, 'Y'));
                printf("\n");
            }
        }
    }

    pause_console();
}

/* =========================
 * TESTE DEMUX 1:2
 * ========================= */

void test_demux2(void)
{
    Circuit c = circuit_demux2();

    printf("\n==============================\n");
    printf(" TESTE: DEMUX 1:2\n");
    printf("==============================\n");

    circuit_dump(&c);

    printf("\nA S | Y0 Y1\n");
    printf("-------------\n");

    for (int a = 0; a <= 1; a++)
    {
        for (int s = 0; s <= 1; s++)
        {
            circuit_set(&c, 'A', a);
            circuit_set(&c, 'S', s);

            circuit_eval(&c);

            printf("%d %d | ", a, s);
            print_bool(circuit_get(&c, 'Y0'));
            printf("  ");
            print_bool(circuit_get(&c, 'Y1'));
            printf("\n");
        }
    }

    pause_console();
}

/* =========================
 * MAIN
 * ========================= */

int main(void)
{
    printf("=== LOGICAL PROCESSOR ===\n");
    printf("Teste de Circuitos Logicos\n");

    test_binary_gate("AND",   circuit_and);
    test_binary_gate("OR",    circuit_or);
    test_binary_gate("NAND",  circuit_nand);
    test_binary_gate("NOR",   circuit_nor);
    test_binary_gate("XOR",   circuit_xor);
    test_binary_gate("XNOR",  circuit_xnor);

    test_not_gate();

    test_mux2();
    test_demux2();

    printf("\nTodos os testes finalizados.\n");
    return 0;
}
