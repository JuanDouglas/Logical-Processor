#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "exp.h"
#include "circuit.h"

CircuitConclusion execute_ctr(Circuit *ctr, bool inputs[NUM_LETTERS])
{
    CircuitConclusion cc = {};

    short count = 0;
    for (short i = 0; i < ctr->count; i++)
    {
        CircuitExpression expr = ctr->expressions[i];

        if (!(expr.created))
        {
            break;
        }

        execute_expr(&expr, inputs);

        count++;
    }

    // Calcular expressões em letras para utilizar em outras expressões, assim encadeando várias expressões.

    return;
}

void add_expression(Circuit *ctr, char name[MAX_EXPRESSION_NAME], char expression[MAX_EXPRESSION])
{
    CircuitExpression expr = createExpr(name, expression);

    int size = sizeof(CircuitExpression);

    CircuitExpression *ref = ctr->expressions;

    // Definindo o valor de expressions[ctr->count] usando expr direto na memória
    memcpy(&ref[ctr->count], &expr, size);

    ctr-> count = (ctr->count) + 1;
}
