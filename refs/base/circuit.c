#include "circuit.h"
#include "exp.h"
#include <string.h>
#include <stdio.h>

/*
 * Ponteiro global para o circuito ativo.
 * Funciona como um barramento interno do processador.
 */
static Circuit *ACTIVE = NULL;

/* Resolver de sinais usado pelo avaliador */
static bool resolve_signal(char s)
{
    if (!ACTIVE) return false;

    for (int i = 0; i < ACTIVE->signal_count; i++)
        if (ACTIVE->signals[i].name == s)
            return ACTIVE->signals[i].value;

    return false;
}

void circuit_init(Circuit *c, const char *name)
{
    memset(c, 0, sizeof(*c));
    strcpy(c->name, name);
}

void circuit_add_signal(Circuit *c, char name)
{
    c->signals[c->signal_count++] = (Signal){name, 0, 0};
}

void circuit_add_expression(Circuit *c, const char *name, char output, const char *expr)
{
    Expression *e = &c->expressions[c->expr_count++];
    strcpy(e->name, name);
    e->output = output;
    strcpy(e->expr, expr);
}

/* Execução combinacional (todas as expressões em paralelo) */
void circuit_eval(Circuit *c)
{
    ACTIVE = c;

    for (int i = 0; i < c->expr_count; i++)
    {
        Expression *e = &c->expressions[i];
        bool result = eval_expression(e->expr, resolve_signal);

        for (int j = 0; j < c->signal_count; j++)
            if (c->signals[j].name == e->output)
                c->signals[j].next = result;
    }

    ACTIVE = NULL;
}

/* Commit síncrono (borda de clock) */
void circuit_commit(Circuit *c)
{
    for (int i = 0; i < c->signal_count; i++)
        c->signals[i].value = c->signals[i].next;
}

void circuit_set(Circuit *c, char sig, bool value)
{
    for (int i = 0; i < c->signal_count; i++)
        if (c->signals[i].name == sig) {
            c->signals[i].value = value;
            c->signals[i].next = value;
        }
}

bool circuit_get(Circuit *c, char sig)
{
    for (int i = 0; i < c->signal_count; i++)
        if (c->signals[i].name == sig)
            return c->signals[i].value;
    return false;
}

/* Dump completo do estado interno */
void circuit_dump(const Circuit *c)
{
    printf("\n[DUMP :: %s]\n", c->name);
    for (int i = 0; i < c->signal_count; i++)
        printf(" %c=%d", c->signals[i].name, c->signals[i].value);
    printf("\n");
}