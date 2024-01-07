#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#define NUM_LETTERS 26
#define MAX_EXPRESSION 3

struct lmodule
{
    bool values[NUM_LETTERS];
    char expression[MAX_EXPRESSION];
};

bool lport(char name, bool a, bool b){
    if (name == '&') // AND
        return a && b;
    else if(name == '*') // NAND
        return !(a && b);
    else if(name == '|') // OR
        return a || b;
    else if(name == '#') // NOR
        return !a && !b;
    else if(name == '`') // XOR
        return (a || b) && !(a && b);
    else if (name == ':') // XNOR
        return a && b || !(a || b);
}

bool get_value(struct lmodule *module, char name){
   if(!isupper(name))
    {
        printf("Utilizado valor fora de escala!");
    } else {
       return module -> values[name - 'A'];
    }
}

void set_value(struct lmodule *module, char name, bool v){
    if(!isupper(name))
    {
        printf("Utilizado valor fora de escala!");
    } else {
        module -> values[name - 'A'] = v;
    }
}

bool execute(struct lmodule *module){
    bool a,b;

    char port = module -> expression[1];
    a = get_value(module,module -> expression[0]);
    b = get_value(module,module -> expression[2]);

    return lport(port,a,b);
}



int main() {
    bool a = 0,
        b = 1,
        r = lport(':',a,b);

    struct lmodule module = {{false} ,"A|B"};
    char letter = 'A';

    set_value(&module,letter,1);

    r = execute(&module);

    printf("Resultado: %d \n",r );

    return 0;
}