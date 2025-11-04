#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "menufunctions.h"
#include "filaprioridade.h"

// 1 se a > b , 0 caso contrario
static int comparar(chamado a, chamado b){
    if(a.prioridade > b.prioridade || (a.prioridade == b.prioridade && a.tempoSimples < b.tempoSimples)){
        return 1;
    }
    return 0;
}

// criar fila de prioridade
filaPrioridade * criarFilaPrioridade(void){
    filaPrioridade * f = (filaPrioridade *)malloc(sizeof(filaPrioridade));
    if(f == NULL){
        // log
        exit(1);
    }
    f->n = 0;
    f->dim = 10; // tamanho inicial
    f->elementos = (chamado*)malloc(f->dim * sizeof(chamado));
    if(f->elementos == NULL){
        // log
        exit(1);
    }
    return f;
}

// inserir na fila de prioridade
void filaPrioridadeInserir(filaPrioridade* f, chamado c){
    if(f->n >= f->dim){
        // realocar memoria
        f->dim *= 2;
        f->elementos = (chamado*)realloc(f->elementos, f->dim * sizeof(chamado));
        if(f->elementos == NULL){
            // log
            exit(1);
        }
    }

    f->elementos[f->n] = c; // coloca o chamado no final
    f->n++;
    int j = f->n - 1;          // posição onde ele vai entrar

    // ordenar por prioridade (maior prioridade primeiro)
    // se prioridades iguais, ordenar por tempo (mais antigo primeiro)
    while (j > 0) {
        int i = (j - 1) / 2; // pai de j

        chamado pai = f->elementos[i];
        chamado filho = f->elementos[j];

        // Se o filho tem prioridade maior (ou mesmo nível, mas é mais antigo), troca
        if (comparar(filho, pai) == 1) {
            f->elementos[i] = filho;
            f->elementos[j] = pai;
            j = i; // sobe o filho
        } else {
            break;
        }
    }
}

// remover da fila de prioridade
chamado filaPrioridadeRemover(filaPrioridade* f) {
    if(f->n == 0){
        // log
        exit(1);
    }

    chamado removido = f->elementos[0];
    f->n--;
    f->elementos[0] = f->elementos[f->n]; // mover último elemento para a raiz

    // reordenar a fila (heapify down)
    int j = 0;
    while (1) {
        int esquerda = 2 * j + 1;
        int direita = 2 * j + 2;
        int maior = j;

        if (esquerda < f->n && comparar(f->elementos[esquerda], f->elementos[maior]))
            maior = esquerda;
        if (direita < f->n && comparar(f->elementos[direita], f->elementos[maior]))
            maior = direita;

        if (maior == j) // nenhum filho maior, para o loop
            break;

        // troca pai com filho maior
        chamado temp = f->elementos[j];
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
    copia->elementos = (chamado*)malloc(copia->dim * sizeof(chamado));
    if (!copia->elementos) exit(1);

    // copia todos os elementos
    for (int i = 0; i < original->n; i++) {
        copia->elementos[i] = original->elementos[i];
    }

    return copia;
}

void filaPrioridadeLiberar(filaPrioridade** f){
    if(f != NULL && *f != NULL){
        free((*f)->elementos);
        free(*f);
        *f = NULL;
    }
}