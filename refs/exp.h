#ifndef EXP_H
#define EXP_H

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#define NUM_LETTERS 26
#define MAX_EXPRESSION 36
#define MAX_EXPRESSION_NAME 3

// Struct que simula uma expressão de um conjunto de porta lógicas
typedef struct expre
{   
    char expression[MAX_EXPRESSION];
    char name[MAX_EXPRESSION_NAME];
    bool last;
    const bool created;
} CircuitExpression;

// Executa a expressão com as entradas definidas por letras sendo I = I - 'A' onde I se refere a posição do item em inputs.
bool execute_expr(CircuitExpression *expr, bool inputs[NUM_LETTERS]);

// Cria uma struct de CircuitExpression usando a própia expressão e um nome de batismo
CircuitExpression create_expr(char name[MAX_EXPRESSION_NAME], char exp[MAX_EXPRESSION]);
#endif