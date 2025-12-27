#ifndef LOADER_H
#define LOADER_H

#include "base/unit.h"

/* Carrega uma Unit completa a partir de um arquivo de definição */
bool loader_load_from_file(const char *filename, Unit *out_unit, const char *target_unit_name);

#endif