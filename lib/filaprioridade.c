#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "all.h"

/// @brief Cria uma nova fila de prioridade.
/// @param comparar Função de comparação para ordenar os elementos.
/// @return Ponteiro para a nova fila de prioridade.
filaPrioridade * criarFilaPrioridade(int (*comparar)(void* a, void* b)){
    filaPrioridade * f = (filaPrioridade *)malloc(sizeof(filaPrioridade));
    if(f == NULL){
        exit(1);
    }
    f->n = 0;
    f->dim = 10; // tamanho inicial
    f->elementos = malloc(f->dim * sizeof(void*));
    f->comparar = comparar;
    if(f->elementos == NULL){
        exit(1);
    }
    return f;
}

/// @brief Insere um elemento na fila de prioridade.
/// @param f Ponteiro para a fila de prioridade.
/// @param d Ponteiro para o dado a ser inserido.
void filaPrioridadeInserir(filaPrioridade* f, void* d){
    if(f->n >= f->dim){
        // realocar memoria
        f->dim *= 2;
        f->elementos = realloc(f->elementos, f->dim * sizeof(void*));
        if(f->elementos == NULL){
            exit(1);
        }
    }

    f->elementos[f->n] = d; // coloca o chamado no final
    f->n++;
    int j = f->n - 1;          // posição onde ele vai entrar

    // ordenar por prioridade (maior prioridade primeiro)
    // se prioridades iguais, ordenar por tempo (mais antigo primeiro)
    while (j > 0) {
        int i = (j - 1) / 2; // pai de j

        void* pai = f->elementos[i];
        void* filho = f->elementos[j];

        // Se o filho tem prioridade maior (ou mesmo nível, mas é mais antigo), troca
        if (f->comparar(filho, pai) == 1) {
            f->elementos[i] = filho;
            f->elementos[j] = pai;
            j = i; // sobe o filho
        } else {
            break;
        }
    }
}

// remover da fila de prioridade
void* filaPrioridadeRemover(filaPrioridade* f) {
    if(f->n == 0){
        // log
        exit(1);
    }

    void* removido = f->elementos[0];
    f->n--;
    f->elementos[0] = f->elementos[f->n]; // mover último elemento para a raiz

    // reordenar a fila (heapify down)
    int j = 0;
    while (1) {
        int esquerda = 2 * j + 1;
        int direita = 2 * j + 2;
        int maior = j;

        if (esquerda < f->n && f->comparar(f->elementos[esquerda], f->elementos[maior]))
            maior = esquerda;
        if (direita < f->n && f->comparar(f->elementos[direita], f->elementos[maior]))
            maior = direita;

        if (maior == j) // nenhum filho maior, para o loop
            break;

        // troca pai com filho maior
        void* temp = f->elementos[j];
        f->elementos[j] = f->elementos[maior];
        f->elementos[maior] = temp;

        j = maior; // desce para o próximo nível
    }
    
    return removido;
}

filaPrioridade* filaPrioridadeCopiar(filaPrioridade* original) {
    if (!original) return NULL;

    filaPrioridade* copia = (filaPrioridade*)malloc(sizeof(filaPrioridade));
    if (!copia) exit(1);

    copia->n = original->n;
    copia->dim = original->dim;
    copia->elementos = malloc(copia->dim * sizeof(void*));
    copia->comparar = original->comparar;
    if (!copia->elementos) exit(1);

    // copia todos os elementos
    for (int i = 0; i < original->n; i++) {
        copia->elementos[i] = original->elementos[i];
    }

    return copia;
}

void filaPrioridadeLiberar(filaPrioridade** f){
    if(f != NULL && *f != NULL){
        for(int i = 0; i < (*f)->n; i++){
            free((*f)->elementos[i]);
        }
        free((*f)->elementos);
        free(*f);
        *f = NULL;
    }
}