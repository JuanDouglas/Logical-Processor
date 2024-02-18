#include "circuit.c"

Circuit create_sum()
{
    Circuit csum = {.name = "SUM"};

    add_expression(&csum, "SSC", "A&C|B&C|A&B\0");
    add_expression(&csum, "SSS", "A`B`C\0");

    return csum;
}

Circuit create_halfsum()
{
    Circuit csum = {.name = "HSUM"};

    add_expression(&csum, "SSC", "A&B\0");
    add_expression(&csum, "SSS", "A`B\0");

    return csum;
}

Circuit create_x4_demux()
{ 
}

Circuit create_x4_mux()
{ 
}