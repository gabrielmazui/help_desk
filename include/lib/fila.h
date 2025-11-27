#ifndef FILA_H
#define FILA_H

#include "all.h"

typedef struct noFila{
    struct noFila* prox;
    void* dado; // informacoes do chamado
}noFila;

typedef struct fila{
    noFila* first;
    noFila* last;
    int n;
    void (*liberar)(void* d);
} fila;

// criar uma fila
fila* criarFila(void (*liberar)(void* d));
// inserir elemento no fim da fila
void filaInserir(fila * f, void* m);

// remover o primeiro elemento da fila
void* filaRetirar(fila * f);

// remover elemento pelo indice
void* filaRetirarIndice(fila * f, int indice);

// liberar fila
void filaLiberar(fila ** f);

void* filaRemoverIndice(fila* f, int indice);
#endif