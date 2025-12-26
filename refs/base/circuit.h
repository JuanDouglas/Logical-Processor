#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <stdbool.h>

#define MAX_SIGNALS     32
#define MAX_EXPRESSIONS 32
#define MAX_NAME        32
#define MAX_EXPR        64

/*
 * Um sinal representa um fio do circuito.
 * value → estado atual
 * next  → estado após o commit do clock
 */
typedef struct
{
    char name;
    bool value;
    bool next;
} Signal;

/*
 * Expressão lógica combinacional
 */
typedef struct
{
    char name[MAX_NAME];
    char output;
    char expr[MAX_EXPR];
} Expression;

/*
 * Circuito lógico completo
 */
typedef struct
{
    char name[MAX_NAME];

    Signal signals[MAX_SIGNALS];
    int signal_count;

    Expression expressions[MAX_EXPRESSIONS];
    int expr_count;
} Circuit;

/* API */
void circuit_init(Circuit *c, const char *name);
void circuit_add_signal(Circuit *c, char name);
void circuit_add_expression(Circuit *c, const char *name, char output, const char *expr);

void circuit_set(Circuit *c, char sig, bool value);
bool circuit_get(Circuit *c, char sig);

void circuit_eval(Circuit *c);    /* combinacional */
void circuit_commit(Circuit *c);  /* borda de clock */

void circuit_dump(const Circuit *c);

#endif
