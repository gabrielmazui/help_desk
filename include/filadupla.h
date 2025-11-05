#ifndef FILADUPLA_H
#define FILADUPLA_H

#include "menufunctions.h"

typedef struct noDuplo{
    chamado chamado;
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
void insereFilaDupla(filaDupla* f, int pos, chamado cham);

// funcao remover da fila duplamente encadeada
chamado removerFilaDupla(filaDupla* f, int pos);

// funcao esvaziar a fila duplamente encadeada
void esvaziarFilaDupla(filaDupla* f);

// funcao para liberar a fila duplamente encadeada
void liberarFilaDupla(filaDupla** f);

#endif