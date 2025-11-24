#include <stdio.h>
#include <stdlib.h>

#include "all.h"

/// @brief Cria uma fila dupla
/// @return Ponteiro para a fila dupla criada
filaDupla* criarFilaDupla(void){
    filaDupla* f = (filaDupla*)malloc(sizeof(filaDupla));
    if(f == NULL){
        // log
        exit(1);
    }
    f->inicio = NULL;
    f->fim = NULL;
    f->n = 0;
    return f;
}

/// @brief Insere um elemento na fila dupla
/// @param f Ponteiro para a fila dupla
/// @param pos Posição onde o elemento deve ser inserido
/// @param dado item a ser inserido
void filaDuplaInserir(filaDupla* f, int pos, void* dado){
    if(f == NULL || pos < 0 || pos > f->n){
        // log
        exit(1);
    }
    noDuplo* novoNo = (noDuplo*)malloc(sizeof(noDuplo));
    if(novoNo == NULL){
        // log
        exit(1);
    }
    novoNo->dado = dado;
    novoNo->prox = NULL;
    novoNo->ant = NULL;

    if(f->n == 0){
        // fila vazia
        f->inicio = novoNo;
        f->fim = novoNo;
    }else if(pos == 0){
        // inserir no inicio
        novoNo->prox = f->inicio;
        f->inicio->ant = novoNo;
        f->inicio = novoNo;
    }else if(pos == f->n){
        // inserir no fim
        novoNo->ant = f->fim;
        f->fim->prox = novoNo;
        f->fim = novoNo;
    }else{
        // inserir no meio
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


/// @brief  Remove um elemento da fila dupla
/// @param f f Ponteiro para a fila dupla
/// @param pos indice do elemento a ser removido (comeca no 0)
/// @return item removido
void* filaDuplaRemover(filaDupla* f, int pos){
    if(f == NULL || f->n == 0 || pos < 0 || pos >= f->n){
        // log
        exit(1);
    }
    noDuplo* removido;
    void* dadoRemovido = NULL;

    if(f->n == 1){
        // remover unico elemento
        removido = f->inicio;
        dadoRemovido = removido->dado;
        f->inicio = NULL;
        f->fim = NULL;
    }else if(pos == 0){
        // remover do inicio
        removido = f->inicio;
        dadoRemovido = removido->dado;
        f->inicio = f->inicio->prox;
        f->inicio->ant = NULL;
    }else if(pos == f->n - 1){
        // remover do fim
        removido = f->fim;
        dadoRemovido = removido->dado;
        f->fim = f->fim->ant;
        f->fim->prox = NULL;
    }else{
        // remover do meio
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
/// @param f Ponteiro para a fila duplamente encadeada
void filaDuplaEsvaziar(filaDupla* f){
    if(f == NULL || f->n == 0){
        // log
        exit(1);
    }
    noDuplo* atual = f->inicio;
    while(atual != NULL){
        noDuplo* temp = atual;
        atual = atual->prox;
        free(temp);
    }
    f->inicio = NULL;
    f->fim = NULL;
    f->n = 0;
}

/// @brief Libera a memória ocupada pela fila dupla
/// @param f Ponteiro duplo para a fila dupla
void filaDuplaLiberar(filaDupla** f){
    if(f == NULL || *f == NULL){
        return;
    }
    noDuplo* atual = (*f)->inicio;
    while(atual != NULL){
        noDuplo* temp = atual;
        atual = atual->prox;
        free(temp->dado);
        free(temp);
    }
    free(*f);
    *f = NULL;
}