#ifndef FILAPRIORIDADE_H
#define FILAPRIORIDADE_H

#include "menufunctions.h"

typedef struct filaPrioridade{
    int n;
    int dim;
    chamado* elementos;
    
} filaPrioridade;

// criar fila de prioridade
filaPrioridade * criarFilaPrioridade(void);

// inserir na fila de prioridade
void filaPrioridadeInserir(filaPrioridade* f, chamado c);

// remover da fila de prioridade
chamado filaPrioridadeRemover(filaPrioridade* f);

// liberar fila de prioridade
void filaPrioridadeLiberar(filaPrioridade** f);

#endif