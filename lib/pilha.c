#include <stdio.h>
#include <stdlib.h>

#include "all.h"

// funcao para criar uma pilha (retorna um ponteiro para ela)
pilha* criarPilha(void (*liberar)(void* d)){
    pilha* p = malloc(sizeof(pilha));
    if(p == NULL){
        exit(1);
    }
    p->n = 0;
    p->dim = 10;
    p->vet = malloc(sizeof(void*) * p->dim);
    if(p->vet == NULL){
        exit(1);
    }

    p->liberar = liberar;

    return p;
}

// inserir elemento no topo (push)
void pushPilha(pilha* p, void* dado){
    if(p->n == p->dim){
        p->dim *= 2;
        void** temp = realloc(p->vet, sizeof(void*)*p->dim);
        if(temp == NULL){
            exit(1);
        }
        p->vet = temp;
    }
    p->vet[(p->n)++] = dado;
}

// remover elemento do topo (pop)
void* popPilha(pilha* p){
    if(p->n == 0){
        exit(1);
    }
    (p->n)--;
    return p->vet[p->n];
}

//visualizar topo da pilha
void* topoPilha(pilha * p){
    if(p->n <= 0){
        exit(1);
    }
    return p->vet[p->n-1];
}

// liberar a pilha
void pilhaLiberar(pilha** p){
    if (p != NULL && *p != NULL) {
        for(int i = 0; i < (*p)->n; i++){
            (*p)->liberar((*p)->vet[i]);
        }
        free((*p)->vet);
        free(*p);
        *p = NULL;
    }
}
