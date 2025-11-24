#include <stdio.h>
#include <stdlib.h>

#include "all.h"

/// @brief Cria um novo nó da árvore binária
/// @param dado Ponteiro para o dado a ser armazenado no nó
/// @return Ponteiro para o novo nó criado
static arvNo* criarNo(void* dado) {
    arvNo* n = malloc(sizeof(arvNo));
    n->dado = dado;
    n->esq = NULL;
    n->dir = NULL;
    return n;
}

/// @brief Função para criar uma árvore binária
/// @param comparar Ponteiro para a função de comparação dos dados
/// @return Ponteiro para a nova árvore binária criada
arvoreBinaria* arv_criar(int (*comparar)(void* a, void* b)) {
    arvoreBinaria* arv = malloc(sizeof(arvoreBinaria));
    arv->raiz = NULL;
    arv->comparar = comparar;
    return arv;
}

/// @brief Função recursiva para inserir um dado na árvore binária
/// @param raiz raiz atual da árvore
/// @param dado Ponteiro para o dado a ser inserido
/// @param comparar Ponteiro para a função de comparação dos dados
/// @return Ponteiro para o nó raiz atualizado
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

/// @brief Função para inserir um dado na árvore binária
/// @param arv Ponteiro para a árvore binária
/// @param dado Ponteiro para o dado a ser inserido
void arv_inserir(arvoreBinaria* arv, void* dado) {
    arv->raiz = inserirRec(arv->raiz, dado, arv->comparar);
}

/// @brief Encontra o nó com o menor valor na árvore binária
/// @param raiz Ponteiro para o nó raiz da árvore
/// @return Ponteiro para o nó com o menor valor
static arvNo* encontrarMinimo(arvNo* raiz) {
    while (raiz && raiz->esq != NULL)
        raiz = raiz->esq;
    return raiz;
}

/// @brief Função recursiva para remover um dado da árvore binária
/// @param raiz Ponteiro para o nó raiz da árvore
/// @param chave Ponteiro para a chave do dado a ser removido
/// @param comparar Ponteiro para a função de comparação dos dados
/// @param removido Ponteiro para armazenar o dado removido
/// @return Ponteiro para o nó raiz atualizado
static arvNo* removerRec(arvNo* raiz, void* chave, int (*comparar)(void*, void*), void** removido) {
    if (raiz == NULL) {
        *removido = NULL; // não encontrou
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

    // nó encontrado: guardamos o dado original
    *removido = raiz->dado;

    // caso 1: nó folha
    if (raiz->esq == NULL && raiz->dir == NULL) {
        free(raiz);
        return NULL;
    }

    // caso 2: nó com apenas um filho
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

    // caso 3: nó com dois filhos
    arvNo* menorDir = encontrarMinimo(raiz->dir);
    raiz->dado = menorDir->dado;  // substitui pelo menor da direita
    raiz->dir = removerRec(raiz->dir, menorDir->dado, comparar, NULL); // não precisa capturar o dado da subárvore

    return raiz;
}

/// @brief Função para remover um dado da árvore binária
/// @param arv Ponteiro para a árvore binária
/// @param chave Ponteiro para a chave do dado a ser removido
/// @return Ponteiro para o dado removido
void* arv_remover(arvoreBinaria* arv, void* chave) {
    void* removido = NULL;
    arv->raiz = removerRec(arv->raiz, chave, arv->comparar, &removido);
    return removido;
}

/// @brief Função recursiva para buscar um dado na árvore binária
/// @param raiz Ponteiro para o nó raiz da árvore
/// @param chave Ponteiro para a chave do dado a ser buscado
/// @param comparar Ponteiro para a função de comparação dos dados
/// @return Ponteiro para o dado encontrado ou NULL se não encontrado
static void* buscarRec(arvNo* raiz, void* chave, int (*comparar)(void*, void*)) {
    if (!raiz) return NULL;

    int cmp = comparar(chave, raiz->dado);

    if (cmp == 0) return raiz->dado;
    if (cmp < 0) return buscarRec(raiz->esq, chave, comparar);
    return buscarRec(raiz->dir, chave, comparar);
}

/// @brief Função para buscar um dado na árvore binária
/// @param arv Ponteiro para a árvore binária
/// @param chave Ponteiro para a chave do dado a ser buscado
/// @return Ponteiro para o dado encontrado ou NULL se não encontrado
void* arv_buscar(arvoreBinaria* arv, void* chave) {
    return buscarRec(arv->raiz, chave, arv->comparar);
}

/// @brief Função recursiva para liberar a memória da árvore binária
/// @param raiz Ponteiro para o nó raiz da árvore
static void liberarRec(arvNo* raiz) {
    if (!raiz) return;
    liberarRec(raiz->esq);
    liberarRec(raiz->dir);
    free(raiz->dado); // libera o dado armazenado
    free(raiz);
}


// void imprimirInOrder(arvNo* no, void (*print)(void*)) {
//     if (no == NULL) return;
//     imprimirInOrder(no->esq, print);
//     print(no->dado);        // chama a função que sabe como imprimir
//     imprimirInOrder(no->dir, print);
// }

/// @brief Função para liberar a memória da árvore binária
/// @param arv Ponteiro para a árvore binária
void arv_liberar(arvoreBinaria** arv) {
    liberarRec((*arv)->raiz);
    free(arv);
}