#include <stdio.h>
#include "refs\circuit.c"

int main()
{

    /*
    EXEMPLO de parte do circuito somador
    Cout = ACin + BCin + AB
    */

    bool letters[NUM_LETTERS] = {1, 0, 1};
    CircuitExpression cExpr = create_expr("SSC", "A&C|B&C|A&B\0");
    execute_expr(&cExpr, letters);

    return 0;
}