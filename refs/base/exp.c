#include "exp.h"

/*
 * Máquina de pilha simples para avaliação lógica.
 * Operadores suportados:
 *  & AND
 *  | OR
 *  ` XOR
 */
bool eval_expression(const char *expr, bool (*resolver)(char))
{
    bool stack[64];
    int sp = 0;

    for (int i = 0; expr[i]; i++)
    {
        char c = expr[i];

        if (c >= 'A' && c <= 'Z')
        {
            stack[sp++] = resolver(c);
        }
        else
        {
            bool b = stack[--sp];
            bool a = stack[--sp];

            switch (c)
            {
                case '&': stack[sp++] = a && b; break;
                case '|': stack[sp++] = a || b; break;
                case '`': stack[sp++] = a ^ b;  break;
            }
        }
    }
    return stack[0];
}
