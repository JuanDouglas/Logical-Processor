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
        return (a && b) || !(a || b);

    return NULL;
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

    return NULL;
}

bool execute_expr(CircuitExpression *expr, bool inputs[NUM_LETTERS])
{
    bool a = false, b, r;

    for (short i = 0; i < expr->expression_count; i += 3)
    {
        char port;

        if (i == 0)
        {
            port = expr->expression[i + 1];
            a = get_value(inputs, expr->expression[i]);
            b = get_value(inputs, expr->expression[i + 2]);
        }
        else if ((i + 3) > expr->expression_count || expr->expression[i + 3] == '\0')
        {
            port = expr->expression[i];
            a = r;
            b = get_value(inputs, expr->expression[i + 1]);
        }
        else
        {
            a = r;
            b = lport(expr->expression[i + 2], get_value(inputs, expr->expression[i + 1]), get_value(inputs, expr->expression[i + 3]));
            port = expr->expression[i];
            i++;
        }

        r = lport(port, a, b);
    }

    expr->last = r;
    return r;
}

CircuitExpression create_expr(char *name, char *exp, short expression_count)
{
    CircuitExpression expr = {.last = false,
                              .created = true,
                              .expression_count = expression_count,
                              .expression = malloc(expression_count * sizeof(char))};

    strncpy(expr.name, name, MAX_EXPRESSION_NAME);
    strncpy(expr.expression, exp, expression_count);

    return expr;
}