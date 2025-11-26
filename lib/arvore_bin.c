#include <stdio.h>
#include <stdlib.h>

#include "all.h"

/// @brief Cria um novo nó da árvore binária
static arvNo* criarNo(void* dado) {
    arvNo* n = malloc(sizeof(arvNo));
    n->dado = dado;
    n->esq = NULL;
    n->dir = NULL;
    return n;
}

/// @brief Cria uma árvore binária
arvoreBinaria* arv_criar(int (*comparar)(void* a, void* b), void (*liberar)(void* d)) {
    arvoreBinaria* arv = malloc(sizeof(arvoreBinaria));
    arv->raiz = NULL;
    arv->comparar = comparar;
    arv->liberar = liberar;
    return arv;
}

/// @brief Inserção recursiva
static arvNo* inserirRec(arvNo* raiz, void* dado, int (*comparar)(void*, void*)) {
    if (raiz == NULL) {
        return criarNo(dado);
    }

    int cmp = comparar(dado, raiz->dado);

    if (cmp < 0) {
        raiz->esq = inserirRec(raiz->esq, dado, comparar);
    } else {
        raiz->dir = inserirRec(raiz->dir, dado, comparar);
    }

    return raiz;
}

void arv_inserir(arvoreBinaria* arv, void* dado) {
    arv->raiz = inserirRec(arv->raiz, dado, arv->comparar);
}

/// @brief Retorna o menor nó da árvore
static arvNo* encontrarMinimo(arvNo* raiz) {
    while (raiz && raiz->esq != NULL)
        raiz = raiz->esq;
    return raiz;
}

/// @brief Remoção recursiva
static arvNo* removerRec(arvNo* raiz, void* chave, int (*comparar)(void*, void*), void** removido) {
    if (raiz == NULL) {
        *removido = NULL;
        return NULL;
    }

    int cmp = comparar(chave, raiz->dado);

    if (cmp < 0) {
        raiz->esq = removerRec(raiz->esq, chave, comparar, removido);
        return raiz;
    } 
    if (cmp > 0) {
        raiz->dir = removerRec(raiz->dir, chave, comparar, removido);
        return raiz;
    }

    // Nó encontrado
    *removido = raiz->dado;

    // Caso 1: folha
    if (raiz->esq == NULL && raiz->dir == NULL) {
        free(raiz);
        return NULL;
    }

    // Caso 2: um filho
    if (raiz->esq == NULL) {
        arvNo* temp = raiz->dir;
        free(raiz);
        return temp;
    }
    if (raiz->dir == NULL) {
        arvNo* temp = raiz->esq;
        free(raiz);
        return temp;
    }

    // Caso 3: dois filhos
    arvNo* menorDir = encontrarMinimo(raiz->dir);
    raiz->dado = menorDir->dado;

    raiz->dir = removerRec(raiz->dir, menorDir->dado, comparar, NULL);

    return raiz;
}

void* arv_remover(arvoreBinaria* arv, void* chave) {
    void* removido = NULL;
    arv->raiz = removerRec(arv->raiz, chave, arv->comparar, &removido);
    return removido;
}

/// @brief Busca recursiva
static void* buscarRec(arvNo* raiz, void* chave, int (*comparar)(void*, void*)) {
    if (!raiz) return NULL;

    int cmp = comparar(chave, raiz->dado);

    if (cmp == 0) return raiz->dado;
    if (cmp < 0) return buscarRec(raiz->esq, chave, comparar);
    return buscarRec(raiz->dir, chave, comparar);
}

void* arv_buscar(arvoreBinaria* arv, void* chave) {
    return buscarRec(arv->raiz, chave, arv->comparar);
}

/// @brief Liberação recursiva
static void liberarRec(arvNo* raiz, void (*liberar)(void*)) {
    if (!raiz) return;

    liberarRec(raiz->esq, liberar);
    liberarRec(raiz->dir, liberar);

    liberar(raiz->dado);
    free(raiz);
}

/// @brief Libera a árvore inteira
void arv_liberar(arvoreBinaria** arv) {
    if (arv == NULL || *arv == NULL) return;

    liberarRec((*arv)->raiz, (*arv)->liberar);

    free(*arv);
    *arv = NULL;
}
