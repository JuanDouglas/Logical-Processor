#include "exp.c"
#define MAX_CIRCUIT_EXPRESSIONS 32
#define MAX_CIRCUIT_NAME 128

typedef struct circuit
{
    char name[MAX_CIRCUIT_NAME];
    CircuitExpression expressions[MAX_CIRCUIT_EXPRESSIONS];
    short count;
} Circuit;

typedef struct cconclusion
{
    char circuits[MAX_CIRCUIT_EXPRESSIONS][MAX_EXPRESSION_NAME];
    bool outputs[MAX_CIRCUIT_EXPRESSIONS];
    const int count;
} CircuitConclusion;

// Executa um circuito com as entradas definidas.
CircuitConclusion execute_ctr(Circuit *ctr, bool letters[NUM_LETTERS]);

// Adiciona uma nova expressão a um circuito já criado.
void add_expression(Circuit *ctr, char name[MAX_EXPRESSION_NAME], char expression[]);