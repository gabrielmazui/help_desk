#ifndef FILA_H
#define FILA_H

#include "menufunctions.h"

typedef struct noFila{
    struct noFila* prox;
    chamado chamado; // informacoes do chamado
}noFila;

typedef struct fila{
    noFila* first;
    noFila* last;
    int n;
} fila;

// criar uma fila
fila* criarFila(void);

// inserir elemento no fim da fila
void filaInserir(fila * f, chamado m);

// remover o primeiro elemento da fila
chamado filaRetirar(fila * f);

// remover elemento pelo indice
chamado filaRetirarIndice(fila * f, int indice);

// liberar fila
void filaLiberar(fila ** f);

#endif