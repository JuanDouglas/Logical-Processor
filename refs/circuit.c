#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "exp.h"
#include "circuit.h"

CircuitConclusion execute_ctr(Circuit *ctr, bool inputs[NUM_LETTERS])
{
    CircuitConclusion cc = {.count = ctr->count};

    short count = 0;
    for (short i = 0; i < ctr->count; i++)
    {
        CircuitExpression expr = ctr->expressions[i];

        if (!(expr.created))
        {
            break;
        }

        execute_expr(&expr, inputs);

        memcpy(&cc.circuits[i], &expr.name, sizeof(char) * MAX_EXPRESSION_NAME);

        cc.outputs[i] = expr.last;
        count++;
    }

    // Calcular expressões em letras para utilizar em outras expressões, assim encadeando várias expressões.

    return cc;
}

void add_expression(Circuit *ctr, char name[MAX_EXPRESSION_NAME], char expression[MAX_EXPRESSION])
{
    CircuitExpression expr = create_expr(name, expression);
    int expressionSize = sizeof(CircuitExpression);

    if (ctr->count > 0)
    {
        ctr->expressions = realloc(ctr->expressions, (ctr->count + 1) * expressionSize);
    }
    else
    {
        ctr->expressions = malloc(expressionSize);
    }

    memcpy(&ctr->expressions[ctr->count], &expr, expressionSize);

    ctr->count = (ctr->count) + 1;
}
