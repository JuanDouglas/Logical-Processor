#include "exp.h"

/*
 * Máquina de pilha para avaliação lógica (RPN).
 * AGORA SUPORTA:
 * - Variáveis: A-Z, a-z, 0-9
 * - Operadores: & (AND), | (OR), ^ (XOR), * (NAND), = (XNOR), ! (NOT)
 */
bool eval_expression(const char *expr, bool (*resolver)(char))
{
    bool stack[64];
    int sp = 0;

    for (int i = 0; expr[i]; i++)
    {
        char c = expr[i];

        // CORREÇÃO: Aceita letras (Maiusc/Minusc) e Números como Sinais
        if ((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9'))
        {
            stack[sp++] = resolver(c);
        }
        else if (c == '!') // Operador Unário
        {
            if (sp < 1) return false;
            bool a = stack[--sp];
            stack[sp++] = !a;
        }
        else // Operadores Binários
        {
            if (sp < 2) return false;
            bool b = stack[--sp];
            bool a = stack[--sp];

            switch (c)
            {
                case '&': stack[sp++] = a && b; break;
                case '|': stack[sp++] = a || b; break;
                case '^': stack[sp++] = a != b; break; // XOR
                case '*': stack[sp++] = !(a && b); break; // NAND
                case '=': stack[sp++] = a == b; break; // XNOR
                    // Se cair aqui com um caractere estranho, ignora
            }
        }
    }
    return (sp > 0) ? stack[0] : false;
}