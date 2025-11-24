#ifndef ARVORE_BIN_H
#define ARVORE_BIN_H

typedef struct arvNo {
    void* dado;
    struct arvNo* esq;
    struct arvNo* dir;
} arvNo;

typedef struct arvoreBinaria {
    struct arvNo* raiz;
    int (*comparar)(void* a, void* b);
} arvoreBinaria;

arvoreBinaria* arv_criar(int (*comparar)(void* a, void* b));

void arv_inserir(arvoreBinaria* arv, void* dado);

void* arv_remover(arvoreBinaria* arv, void* chave);

void* arv_buscar(arvoreBinaria* arv, void* chave);

void arv_liberar(arvoreBinaria** arv);

#endif