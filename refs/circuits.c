#include "circuits.h"

/*
 * Convenção:
 * Entradas  → A, B, S
 * Saídas    → Y, Y0, Y1
 * Operadores (exp.h):
 *   &  AND
 *   |  OR
 *   !  NOT
 *   *  NAND
 *   ^  XOR
 *   =  XNOR
 */

/* =========================
 * PORTAS BÁSICAS
 * ========================= */

Circuit circuit_and(void)
{
    Circuit c;
    circuit_init(&c, "AND");

    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'B');
    circuit_add_signal(&c, 'Y');

    circuit_add_expression(&c, "Y = A & B", 'Y', "AB&");
    return c;
}

Circuit circuit_or(void)
{
    Circuit c;
    circuit_init(&c, "OR");

    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'B');
    circuit_add_signal(&c, 'Y');

    circuit_add_expression(&c, "Y = A | B", 'Y', "AB|");
    return c;
}

Circuit circuit_not(void)
{
    Circuit c;
    circuit_init(&c, "NOT");

    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'Y');

    circuit_add_expression(&c, "Y = !A", 'Y', "A!");
    return c;
}

Circuit circuit_nand(void)
{
    Circuit c;
    circuit_init(&c, "NAND");

    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'B');
    circuit_add_signal(&c, 'Y');

    circuit_add_expression(&c, "Y = !(A & B)", 'Y', "AB*");
    return c;
}

Circuit circuit_nor(void)
{
    Circuit c;
    circuit_init(&c, "NOR");

    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'B');
    circuit_add_signal(&c, 'Y');

    circuit_add_expression(&c, "Y = !(A | B)", 'Y', "AB|!");
    return c;
}

Circuit circuit_xor(void)
{
    Circuit c;
    circuit_init(&c, "XOR");

    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'B');
    circuit_add_signal(&c, 'Y');

    circuit_add_expression(&c, "Y = A ^ B", 'Y', "AB^");
    return c;
}

Circuit circuit_xnor(void)
{
    Circuit c;
    circuit_init(&c, "XNOR");

    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'B');
    circuit_add_signal(&c, 'Y');

    circuit_add_expression(&c, "Y = A = B", 'Y', "AB=");
    return c;
}

/* =========================
 * MUX 2:1
 * Y = (!S & A) | (S & B)
 * ========================= */

Circuit circuit_mux2(void)
{
    Circuit c;
    circuit_init(&c, "MUX2");

    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'B');
    circuit_add_signal(&c, 'S');
    circuit_add_signal(&c, 'Y');

    /* (!S & A) | (S & B) */
    circuit_add_expression(
        &c,
        "Y = (!S & A) | (S & B)",
        'Y',
        "S!A&SB&|"
    );

    return c;
}

/* =========================
 * DEMUX 1:2
 * Y0 = A & !S
 * Y1 = A & S
 * ========================= */

Circuit circuit_demux2(void)
{
    Circuit c;
    circuit_init(&c, "DEMUX2");

    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'S');
    circuit_add_signal(&c, 'Y0');
    circuit_add_signal(&c, 'Y1');

    circuit_add_expression(
        &c,
        "Y0 = A & !S",
        'Y0',
        "AS!&"
    );

    circuit_add_expression(
        &c,
        "Y1 = A & S",
        'Y1',
        "AS&"
    );

    return c;
}

Circuit circuit_half_adder(void)
{
    Circuit c;
    circuit_init(&c, "HALF_ADDER");

    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'B');
    circuit_add_signal(&c, 'S'); // Sum
    circuit_add_signal(&c, 'C'); // Carry

    circuit_add_expression(&c, "S = A ^ B", 'S', "AB^");
    circuit_add_expression(&c, "C = A & B", 'C', "AB&");

    return c;
}

Circuit circuit_full_adder(void)
{
    Circuit c;
    circuit_init(&c, "FULL_ADDER");

    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'B');
    circuit_add_signal(&c, 'I'); // Cin
    circuit_add_signal(&c, 'S'); // Sum
    circuit_add_signal(&c, 'C'); // Cout

    // S = A ^ B ^ I
    circuit_add_expression(&c, "S = A ^ B ^ I", 'S', "AB^I^");

    // C = (A & B) | (I & (A ^ B))
    circuit_add_expression(&c, "C = (A & B) | (I & (A ^ B))", 'C', "AB&IAB^^|");

    return c;
}

Circuit circuit_register_1bit(void)
{
    Circuit c;
    circuit_init(&c, "REG_1BIT");

    circuit_add_signal(&c, 'D');
    circuit_add_signal(&c, 'Q');
    circuit_add_signal(&c, 'E'); // Enable (clock)

    // Q_next = (E & D) | (!E & Q)
    circuit_add_expression(
        &c,
        "Q = (E & D) | (!E & Q)",
        'Q',
        "ED&EQ!&|"
    );

    return c;
}
