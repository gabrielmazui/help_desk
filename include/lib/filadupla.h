#ifndef FILADUPLA_H
#define FILADUPLA_H

#include "all.h"

typedef struct noDuplo{
    void* dado;
    struct noDuplo* prox;
    struct noDuplo* ant;
} noDuplo;

typedef struct filaDupla{
    noDuplo* inicio;
    noDuplo* fim;
    int n;
} filaDupla;

// funcao criar fila duplamente encadeada
filaDupla* criarFilaDupla(void);

// funcao inserir na fila duplamente encadeada
void filaDuplaInserir(filaDupla* f, int pos, void* dado);

// funcao remover da fila duplamente encadeada
void* filaDuplaRemover(filaDupla* f, int pos);

// funcao esvaziar a fila duplamente encadeada
void filaDuplaEsvaziar(filaDupla* f);

// funcao para liberar a fila duplamente encadeada
void filaDuplaLiberar(filaDupla** f);

#endif