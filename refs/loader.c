#include "loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_LIB_SIZE 32

/* Biblioteca temporária para guardar os CIRCUITs lidos antes de usá-los na UNIT */
static Circuit lib_circuits[MAX_LIB_SIZE];
static int lib_count = 0;

static void register_circuit(Circuit c) {
    if (lib_count < MAX_LIB_SIZE) lib_circuits[lib_count++] = c;
}

static Circuit* find_circuit(const char *name) {
    for(int i=0; i<lib_count; i++)
        if(strcmp(lib_circuits[i].name, name) == 0) return &lib_circuits[i];
    return NULL;
}

/* Parsers Auxiliares */
void trim(char *s) {
    char *p = s;
    int l = strlen(p);
    while(l > 0 && (p[l-1] == '\n' || p[l-1] == '\r' || p[l-1] == ' ')) p[--l] = 0;
}

/* Core Loader */
bool loader_load_from_file(const char *filename, Unit *out_unit, const char *target_unit_name) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("Erro: Nao abriu %s\n", filename); return false; }

    char line[MAX_LINE];
    char mode[10] = "NONE"; // NONE, CIRCUIT, UNIT

    Circuit temp_c;
    Unit temp_u;

    // Nomes das instancias dentro da Unit (ex: "u0" -> index 0)
    struct { char name[32]; int idx; } instances[MAX_SUBCIRCUITS];
    int inst_count = 0;

    lib_count = 0; // Reset library

    while (fgets(line, sizeof(line), f)) {
        trim(line);
        if (line[0] == '#' || line[0] == 0) continue; // Skip comment/empty

        char cmd[32], arg1[64], arg2[64], arg3[64];
        int tokens = sscanf(line, "%[^:]:%s %s %s", cmd, arg1, arg2, arg3);

        // --- MODO: CIRCUIT ---
        if (strcmp(cmd, "CIRCUIT") == 0) {
            strcpy(mode, "CIRCUIT");
            circuit_init(&temp_c, arg1); // arg1 = Nome
        }
        else if (strcmp(mode, "CIRCUIT") == 0) {
            if (strcmp(cmd, "PINS") == 0) {
                // Parse manual de pinos separados por espaço
                char *ptr = line + 6; // Pula "PINS: "
                char *token = strtok(ptr, " ");
                while(token) {
                    // Detecta se é char único ou não. Nosso sistema só aceita char único para sinais internos!
                    // Para simplificar, assumimos que o usuário usa A, B, C... no arquivo
                    circuit_add_signal(&temp_c, token[0]);
                    token = strtok(NULL, " ");
                }
            }
            else if (strcmp(cmd, "RPN") == 0) {
                // RPN: Y = AB&
                // arg1=Y, arg2="=", arg3="AB&" (sscanf pegou a string)
                // Precisamos ler o resto da linha crua para pegar a RPN correta se tiver espaços (embora RPN n deva ter)
                char output = arg1[0];
                char *eq_pos = strchr(line, '=');
                if (eq_pos) {
                    char rpn[64];
                    sscanf(eq_pos + 1, "%s", rpn);
                    circuit_add_expression(&temp_c, "FILE_EXPR", output, rpn);
                }
            }
            else if (strcmp(cmd, "END") == 0) {
                register_circuit(temp_c);
                strcpy(mode, "NONE");
            }
        }

        // --- MODO: UNIT ---
        else if (strcmp(cmd, "UNIT") == 0) {
            // Se encontrarmos a unit que o usuário quer, começamos a preencher
            if (strcmp(arg1, target_unit_name) == 0) {
                strcpy(mode, "UNIT");
                unit_init(&temp_u, arg1);
                inst_count = 0;
            } else {
                strcpy(mode, "SKIP"); // Ignora units que não pedimos
            }
        }
        else if (strcmp(mode, "UNIT") == 0) {
            if (strcmp(cmd, "USE") == 0) {
                // USE: ALU_SLICE u0
                Circuit *proto = find_circuit(arg1);
                if (proto) {
                    unit_add_component(&temp_u, *proto);
                    // Salva nome da instancia para linkar depois
                    strcpy(instances[inst_count].name, arg2);
                    instances[inst_count].idx = inst_count;
                    inst_count++;
                } else {
                    printf("Erro: Circuito '%s' nao definido antes.\n", arg1);
                }
            }
            else if (strcmp(cmd, "WIRE") == 0) {
                // WIRE: u0.C -> u1.I
                // WIRE: IN.a -> u0.A
                char src_part[32], arrow[4], dst_part[32];
                sscanf(line, "WIRE: %s %s %s", src_part, arrow, dst_part);

                // Parse Source (Instancia.Pino)
                char src_inst[32], src_pin_str[4];
                char *dot1 = strchr(src_part, '.');
                *dot1 = 0;
                strcpy(src_inst, src_part);
                strcpy(src_pin_str, dot1 + 1);

                // Parse Dest (Instancia.Pino)
                char dst_inst[32], dst_pin_str[4];
                char *dot2 = strchr(dst_part, '.');
                *dot2 = 0;
                strcpy(dst_inst, dst_part);
                strcpy(dst_pin_str, dot2 + 1);

                int src_idx = -2, dst_idx = -2;

                // Resolver indices
                if (strcmp(src_inst, "IN") == 0) src_idx = -1;
                else if (strcmp(src_inst, "OUT") == 0) src_idx = -1; // Raro usar OUT como source
                else {
                    for(int i=0; i<inst_count; i++)
                        if(strcmp(instances[i].name, src_inst)==0) src_idx = instances[i].idx;
                }

                if (strcmp(dst_inst, "OUT") == 0) dst_idx = -1;
                else {
                    for(int i=0; i<inst_count; i++)
                        if(strcmp(instances[i].name, dst_inst)==0) dst_idx = instances[i].idx;
                }

                if (src_idx != -2 && dst_idx != -2) {
                    unit_add_wire(&temp_u, src_idx, src_pin_str[0], dst_idx, dst_pin_str[0]);
                } else {
                    printf("Erro WIRE: Instancia nao encontrada (%s -> %s)\n", src_inst, dst_inst);
                }
            }
            else if (strcmp(cmd, "END") == 0) {
                *out_unit = temp_u;
                fclose(f);
                return true; // Sucesso
            }
        }
    }

    fclose(f);
    return false;
}