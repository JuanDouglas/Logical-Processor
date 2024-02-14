#include <stdio.h>
#include "refs\circuit.c"

int main()
{

    /*
    EXEMPLO de parte do circuito somador
    Cout = ACin + BCin + AB
    */

    bool letters[NUM_LETTERS] = {1, 0, 1}; 
    Circuit csum = {};

    add_expression(&csum, "SSC", "A&C|B&C|A&B\0");
    add_expression(&csum, "SSS", "A`B`C\0");

    execute_ctr(&csum, letters);
    return 0;
}