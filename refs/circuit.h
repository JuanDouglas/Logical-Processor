#include "exp.c"
#define MAX_EXPRESSION_CIRCUIT 32

typedef struct circuit
{
    CircuitExpression expressions[MAX_EXPRESSION_CIRCUIT];
    short count;
} Circuit;

typedef struct cconclusion
{
    char circuits[MAX_EXPRESSION_CIRCUIT][MAX_EXPRESSION_NAME];
    bool outputs[MAX_EXPRESSION_CIRCUIT];
} CircuitConclusion;

// Executa um circuito com as entradas definidas.
CircuitConclusion execute_ctr(Circuit *ctr, bool letters[NUM_LETTERS]);

// Adiciona uma nova expressão a um circuito já criado.
void add_expression(Circuit *ctr, char name[MAX_EXPRESSION_NAME], char expression[MAX_EXPRESSION]);