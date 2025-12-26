#ifndef VIEWER_H
#define VIEWER_H

#include "circuit.h"

/* Inicializa o visualizador */
void viewer_init(void);

/* Renderiza todos os circuitos */
void viewer_render(Circuit *circuits, int count);

/* Lê comandos do usuário */
char viewer_input(void);

/* Limpa tela */
void viewer_clear(void);

#endif
