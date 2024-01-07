#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#define NUM_LETTERS 26
#define MAX_EXPRESSION 36

struct lmodule
{
    bool values[NUM_LETTERS];
    char expression[MAX_EXPRESSION];
};

// Calcula a saida de uma porta lógica
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
        return (a || b) && !((a && b) || (!a && !b));
    else if (name == ':') // XNOR
        return a && b || !(a || b);
}

// Obtém o valor de um membro da expressão
bool get_value(struct lmodule *module, char name){
   if(!isupper(name))
    {
        printf("Utilizado valor fora de escala!");
    } else {
       return module -> values[name - 'A'];
    }
}

// Define o valor de um membro da expressão.
void set_value(struct lmodule *module, char name, bool v){
    if(!isupper(name))
    {
        printf("Utilizado valor fora de escala!");
    } else {
        module -> values[name - 'A'] = v;
    }
}

// Função para executar a expressão de portas lógicas na struct lmodule
bool execute(struct lmodule *module) {
    bool a = false, b, r;

    for (short i = 0; module->expression[i] != '\0'; i += 3) {
        char port = (module->expression[i + 2] == '\0') ? module->expression[i] : module->expression[i + 2];

        if (i > 0) {
            a = (i % 3 == 0) ? r : get_value(module, module->expression[i + 1]);
            b = get_value(module, module->expression[i + 3]);
        } else {
            a = get_value(module, (r = lport(port, get_value(module, module->expression[i]), get_value(module, module->expression[i + 2]))));
            continue;
        }

        r = (i % 3 == 0) ? lport(module->expression[i], r, lport(port, a, b)) : lport(port, r, lport(module->expression[i], a, b));
    }

    return r;
}

int main() {

    /*
    EXEMPLO de parte do circuito somador
    Cout = ACin + BCin + AB
    */ 
    struct lmodule module = {{0,1,1} ,"A&C|B&C|A&B"};

    bool r = execute(&module);

    return 0;
}