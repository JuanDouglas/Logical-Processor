#include "circuit.c"

Circuit create_sum()
{
    Circuit csum = {.name = "SUM"};

    add_expression(&csum, "SSC", "A&C|B&C|A&B");
    add_expression(&csum, "SSS", "A`B`C");

    return csum;
}

Circuit create_halfsum()
{
    Circuit csum = {.name = "HSUM"};

    add_expression(&csum, "SSC", "A&B");
    add_expression(&csum, "SSS", "A`B");

    return csum;
}

Circuit create_x4_demux()
{ 
    return;
}

Circuit create_x4_mux()
{ 
    return;
}