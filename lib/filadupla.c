#include <stdio.h>
#include <stdlib.h>

#include "all.h"

/// @brief Cria uma fila dupla
/// @return Ponteiro para a fila dupla criada
filaDupla* criarFilaDupla(void (*liberar)(void* d)){
    filaDupla* f = (filaDupla*)malloc(sizeof(filaDupla));
    if(f == NULL){
        exit(1);
    }
    f->inicio = NULL;
    f->fim = NULL;
    f->n = 0;
    f->liberar = liberar;
    return f;
}


/// @brief Insere um elemento na fila dupla
void filaDuplaInserir(filaDupla* f, int pos, void* dado){
    if(f == NULL || pos < 0 || pos > f->n){
        exit(1);
    }
    noDuplo* novoNo = (noDuplo*)malloc(sizeof(noDuplo));
    if(novoNo == NULL){
        exit(1);
    }
    novoNo->dado = dado;
    novoNo->prox = NULL;
    novoNo->ant = NULL;

    if(f->n == 0){
        f->inicio = novoNo;
        f->fim = novoNo;
    }else if(pos == 0){
        novoNo->prox = f->inicio;
        f->inicio->ant = novoNo;
        f->inicio = novoNo;
    }else if(pos == f->n){
        novoNo->ant = f->fim;
        f->fim->prox = novoNo;
        f->fim = novoNo;
    }else{
        noDuplo* atual = f->inicio;
        for(int i = 0; i < pos; i++){
            atual = atual->prox;
        }
        novoNo->prox = atual;
        novoNo->ant = atual->ant;
        atual->ant->prox = novoNo;
        atual->ant = novoNo;
    }
    f->n++;
}

/// @brief Remove elemento da fila dupla
void* filaDuplaRemover(filaDupla* f, int pos){
    if(f == NULL || f->n == 0 || pos < 0 || pos >= f->n){
        exit(1);
    }

    noDuplo* removido;
    void* dadoRemovido;

    if(f->n == 1){
        removido = f->inicio;
        dadoRemovido = removido->dado;
        f->inicio = NULL;
        f->fim = NULL;
    }else if(pos == 0){
        removido = f->inicio;
        dadoRemovido = removido->dado;
        f->inicio = f->inicio->prox;
        f->inicio->ant = NULL;
    }else if(pos == f->n - 1){
        removido = f->fim;
        dadoRemovido = removido->dado;
        f->fim = f->fim->ant;
        f->fim->prox = NULL;
    }else{
        noDuplo* atual = f->inicio;
        for(int i = 0; i < pos; i++){
            atual = atual->prox;
        }
        removido = atual;
        dadoRemovido = removido->dado;
        atual->ant->prox = atual->prox;
        atual->prox->ant = atual->ant;
    }

    free(removido);
    f->n--;
    return dadoRemovido;
}

/// @brief Esvazia a fila duplamente encadeada
void filaDuplaEsvaziar(filaDupla* f){
    if(f == NULL || f->n == 0){
        exit(1);
    }
    noDuplo* atual = f->inicio;
    while(atual != NULL){
        noDuplo* temp = atual;
        atual = atual->prox;

        f->liberar(temp->dado);
        free(temp);
    }
    f->inicio = NULL;
    f->fim = NULL;
    f->n = 0;
}

/// @brief Libera toda a fila dupla
void filaDuplaLiberar(filaDupla** f){
    if(f == NULL || *f == NULL){
        return;
    }
    noDuplo* atual = (*f)->inicio;
    while(atual != NULL){
        noDuplo* temp = atual;
        atual = atual->prox;

        (*f)->liberar(temp->dado);
        free(temp);
    }
    free(*f);
    *f = NULL;
}
