#ifndef EXP_H
#define EXP_H

#include <stdbool.h>

/*
 * Avalia uma expressão lógica em notação pós-fixa.
 * O resolver fornece o valor atual de cada sinal (A–Z).
 *
 * Exemplo de expressão:
 *   "AB&"  → A AND B
 *   "AB`"  → A XOR B
 */
bool eval_expression(const char *expr, bool (*resolver)(char));

#endif
