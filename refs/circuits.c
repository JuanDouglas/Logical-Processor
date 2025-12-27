#include "circuits.h"

#include <stdio.h>

/*
 * Convenção de Operadores (exp.h):
 * &  AND
 * |  OR
 * !  NOT
 * * NAND
 * ^  XOR
 * =  XNOR
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
    circuit_add_expression(&c, "Y = (!S & A) | (S & B)", 'Y', "S!A&SB&|");
    return c;
}

/* =========================
 * DEMUX 1:2 (CORRIGIDO)
 * Saídas renomeadas para '0' e '1' (single char)
 * ========================= */
Circuit circuit_demux2(void)
{
    Circuit c;
    circuit_init(&c, "DEMUX2");

    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'S');

    // CORREÇÃO: Usar char único
    circuit_add_signal(&c, '0'); // Era 'Y0'
    circuit_add_signal(&c, '1'); // Era 'Y1'

    circuit_add_expression(&c, "Y0 = A & !S", '0', "AS!&");
    circuit_add_expression(&c, "Y1 = A & S",  '1', "AS&");

    return c;
}

/* =========================
 * ADDERS
 * ========================= */

Circuit circuit_half_adder(void)
{
    Circuit c;
    circuit_init(&c, "HALF_ADDER");
    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'B');
    circuit_add_signal(&c, 'S');
    circuit_add_signal(&c, 'C');
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

    circuit_add_expression(&c, "S = A ^ B ^ I", 'S', "AB^I^");
    circuit_add_expression(&c, "C = (A & B) | (I & (A ^ B))", 'C', "AB&IAB^&|");
    return c;
}

/* =========================
 * ALU SLICE (ADICIONADO)
 * Fatia de 1 bit da ULA
 * ========================= */
Circuit circuit_alu_slice(void)
{
    Circuit c;
    circuit_init(&c, "ALU_SLICE");

    // 1. Inputs e Outputs Externos
    circuit_add_signal(&c, 'A');
    circuit_add_signal(&c, 'B');
    circuit_add_signal(&c, 'I'); // Cin
    circuit_add_signal(&c, '0'); // S0
    circuit_add_signal(&c, '1'); // S1
    circuit_add_signal(&c, 'Y'); // Resultado
    circuit_add_signal(&c, 'C'); // Cout

    // 2. CORREÇÃO CRÍTICA: Sinais Internos Intermediários
    // Sem isso, o resultado de A^B se perde antes de calcular o Carry!
    circuit_add_signal(&c, 'N'); // AND temp
    circuit_add_signal(&c, 'O'); // OR temp
    circuit_add_signal(&c, 'X'); // XOR temp
    circuit_add_signal(&c, 'S'); // SUM temp

    // 3. Expressões Lógicas
    circuit_add_expression(&c, "N=A&B",   'N', "AB&");
    circuit_add_expression(&c, "O=A|B",   'O', "AB|");
    circuit_add_expression(&c, "X=A^B",   'X', "AB^");
    circuit_add_expression(&c, "S=X^I",   'S', "XI^");

    // Cout depende de X. Agora X existe, então funcionará.
    circuit_add_expression(&c, "C=(A&B)|(I&X)", 'C', "AB&IX&|");

    // 4. Mux de Saída
    circuit_add_expression(
        &c,
        "MUX_OUT",
        'Y',
        "1!0!&N&"  // AND
        "1!0&O&|"  // OR
        "10!&X&|"  // XOR
        "10&S&|"   // ADD
    );

    return c;
}

Circuit circuit_register_1bit(void)
{
    Circuit c;
    circuit_init(&c, "REG_1BIT");
    circuit_add_signal(&c, 'D');
    circuit_add_signal(&c, 'Q');
    circuit_add_signal(&c, 'E');
    circuit_add_expression(&c, "Q = (E & D) | (!E & Q)", 'Q', "ED&EQ!&|");
    return c;
}

Circuit circuit_register_4bit(void) {
    Circuit c;
    circuit_init(&c, "REG_4BIT");

    // Inputs
    circuit_add_signal(&c, 'E'); // Enable (Write)
    circuit_add_signal(&c, 'C'); // Clock (Usado internamente para sincronia)

    // Barramento de Dados (D0..D3 Input, Q0..Q3 Output)
    char inputs[] = {'d', 'c', 'b', 'a'}; // LSB 'a' (bit 0) a 'd' (bit 3)
    char outputs[] = {'3', '2', '1', '0'}; // LSB '0' a '3'

    for(int i=0; i<4; i++) {
        circuit_add_signal(&c, inputs[i]);  // Input D
        circuit_add_signal(&c, outputs[i]); // Output Q

        // Expressão do Latch D com Enable:
        // Q_next = (Enable & D) | (!Enable & Q)
        // Se Enable=1, copia D. Se Enable=0, mantém Q anterior.
        char expr[64];
        char rpn[64];

        // Ex: "0 = (E & a) | (!E & 0)"
        sprintf(expr, "%c = (E & %c) | (!E & %c)", outputs[i], inputs[i], outputs[i]);

        // RPN: "E [input] & E ! [output] & |"
        // Nota: inputs[i] é o bit de dados, outputs[i] é o estado atual
        sprintf(rpn, "E%c&E!%c&|", inputs[i], outputs[i]);

        circuit_add_expression(&c, expr, outputs[i], rpn);
    }

    return c;
}