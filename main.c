#include <stdio.h>
#include "refs\circuits.c"

int main()
{

    /*
    EXEMPLO de parte do circuito somador
    Cout = ACin + BCin + AB
    */

    bool letters[NUM_LETTERS] = {1, 0, 1};

    Circuit csum = create_sum();
    execute_ctr(&csum, letters);

    return 0;
}