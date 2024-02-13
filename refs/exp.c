#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "exp.h"

// Calcula a saida de uma porta lógica
bool lport(char name, bool a, bool b)
{
    if (name == '&') // AND
        return a && b;
    else if (name == '*') // NAND
        return !(a && b);
    else if (name == '|') // OR
        return a || b;
    else if (name == '#') // NOR
        return !a && !b;
    else if (name == '`') // XOR
        return (a || b) && !((a && b) || (!a && !b));
    else if (name == ':') // XNOR
        return a && b || !(a || b);
}

// Obtém o valor de um membro da expressão
bool get_value(bool inputs[NUM_LETTERS], char name)
{
    if (!isupper(name))
    {
        printf("Utilizado valor fora de escala!");
    }
    else
    {
        return inputs[name - 'A'];
    }
}

// Função para executar a expressão de portas lógicas na struct expr
bool execute_expr(CircuitExpression *expr, bool inputs[NUM_LETTERS])
{
    bool a = false, b, r;

    for (short i = 0; expr->expression[i] != '\0'; i += 3)
    {
        char port = (expr->expression[i + 2] == '\0') ? expr->expression[i] : expr->expression[i + 2];

        if (i > 0)
        {
            a = (i % 3 == 0) ? r : get_value(inputs, expr->expression[i + 1]);
            b = get_value(inputs, expr->expression[i + 3]);
        }
        else
        {
            a = get_value(inputs, (r = lport(port, get_value(inputs, expr->expression[i]), get_value(inputs, expr->expression[i + 2]))));
            continue;
        }

        r = (i % 3 == 0) ? lport(expr->expression[i], r, lport(port, a, b)) : lport(port, r, lport(expr->expression[i], a, b));
    }

    expr->last = r;
    return r;
}

CircuitExpression createExpr(char *name, char *exp)
{
    CircuitExpression expr = {.last = false,
                              .created = true};
                              
    strncpy(expr.expression, exp, MAX_EXPRESSION);
    strncpy(expr.name, name, MAX_EXPRESSION_NAME);

    return expr;
}