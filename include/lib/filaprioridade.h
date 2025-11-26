#ifndef FILAPRIORIDADE_H
#define FILAPRIORIDADE_H

#include "all.h"

typedef struct filaPrioridade{
    int n;
    int dim;
    void** elementos;
    int (*comparar)(void* a, void* b);
    void (*liberar)(void* d);
} filaPrioridade;

// criar fila de prioridade
filaPrioridade * criarFilaPrioridade(int (*comparar)(void* a, void* b), void (*liberar)(void* d));

// inserir na fila de prioridade
void filaPrioridadeInserir(filaPrioridade* f, void* d);

// remover da fila de prioridade
void* filaPrioridadeRemover(filaPrioridade* f);

// copiar fila de prioridade
filaPrioridade* filaPrioridadeCopiar(filaPrioridade* original);

// liberar fila de prioridade
void filaPrioridadeLiberar(filaPrioridade** f);

#endif