#include <stdio.h>
#include <stdlib.h>

#include "menufunctions.h"
#include "filadupla.h"

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

void insereFilaDupla(filaDupla* f, int pos, void* dado){
    if(f == NULL || pos < 0 || pos > f->n){
        // log
        exit(1);
    }
    noDuplo* novoNo = (noDuplo*)malloc(sizeof(noDuplo));
    if(novoNo == NULL){
        // log
        exit(1);
    }
    novoNo->chamado = *(chamado*)dado;
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

chamado removerFilaDupla(filaDupla* f, int pos){
    if(f == NULL || f->n == 0 || pos < 0 || pos >= f->n){
        // log
        exit(1);
    }
    noDuplo* removido;
    chamado chamRemovido;

    if(f->n == 1){
        // remover unico elemento
        removido = f->inicio;
        chamRemovido = removido->chamado;
        f->inicio = NULL;
        f->fim = NULL;
    }else if(pos == 0){
        // remover do inicio
        removido = f->inicio;
        chamRemovido = removido->chamado;
        f->inicio = f->inicio->prox;
        f->inicio->ant = NULL;
    }else if(pos == f->n - 1){
        // remover do fim
        removido = f->fim;
        chamRemovido = removido->chamado;
        f->fim = f->fim->ant;
        f->fim->prox = NULL;
    }else{
        // remover do meio
        noDuplo* atual = f->inicio;
        for(int i = 0; i < pos; i++){
            atual = atual->prox;
        }
        removido = atual;
        chamRemovido = removido->chamado;
        atual->ant->prox = atual->prox;
        atual->prox->ant = atual->ant;
    }
    free(removido);
    f->n--;
    return chamRemovido;
}

void liberarFilaDupla(filaDupla** f){
    if(f == NULL || *f == NULL){
        return;
    }
    noDuplo* atual = (*f)->inicio;
    while(atual != NULL){
        noDuplo* temp = atual;
        atual = atual->prox;
        free(temp);
    }
    free(*f);
    *f = NULL;
}