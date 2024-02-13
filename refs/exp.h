#ifndef EXP_H // Verifica se EXP_H não está definido
#define EXP_H // Define EXP_H

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#define NUM_LETTERS 26
#define MAX_EXPRESSION 36
#define MAX_EXPRESSION_NAME 3

// Struct que simula uma expressão para um conjunto de porta lógicas
typedef struct expre
{   
    char expression[MAX_EXPRESSION];
    char name[MAX_EXPRESSION_NAME];
    bool last;
    const bool created;
} CircuitExpression;

// Função para executar a expressão de portas lógicas na struct expr
bool execute_expr(CircuitExpression *expr, bool inputs[NUM_LETTERS]);

CircuitExpression createExpr(char name[MAX_EXPRESSION_NAME], char exp[MAX_EXPRESSION]);
#endif // Fim do bloco #ifndef