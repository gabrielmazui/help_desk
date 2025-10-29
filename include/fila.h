#ifndef FILA_H
#define FILA_H

#include "menufunctions.h"

typedef struct noFila noFila;

typedef struct fila{
    noFila* first;
    noFila* last;
} fila;

// criar uma fila
fila* criarFila(void);
// inserir elemento no fim da fila
void filaInserir(fila * f, chamado m);
// remover o primeiro elemento da fila
chamado filaRetirar(fila * f);
// liberar fila
void filaLiberar(fila ** f);

#endif