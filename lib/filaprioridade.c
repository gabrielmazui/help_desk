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
    int j = f->n;          // posição onde ele vai entrar

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
chamado filaPrioridadeRemover(filaPrioridade* f){
    if(f->n == 0){
        // log
        exit(1);
    }

    chamado removido = f->elementos[0];
    f->n--;
    f->elementos[0] = f->elementos[f->n];

    // reordenar a fila
    int j = 0;
    while (1) {
        int esquerda = 2 * j + 1;
        int direita = 2 * j + 2;
        int pai = j;

        if (esquerda < f->n && comparar(f->elementos[esquerda], f->elementos[pai]))
            pai = esquerda;
        if (direita < f->n && comparar(f->elementos[direita], f->elementos[pai]))
            pai = direita;
        
        if(pai != j){
            // troca
            chamado temp = f->elementos[j];
            f->elementos[j] = f->elementos[pai];
            f->elementos[pai] = temp;
            j = pai;
        }
    }

    return removido;
}

void filaPrioridadeLiberar(filaPrioridade** f){
    if(f != NULL && *f != NULL){
        free((*f)->elementos);
        free(*f);
        *f = NULL;
    }
}