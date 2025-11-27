#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "all.h"

/// @brief Cria uma fila
/// @return Ponteiro para a fila criada
fila * criarFila(void (*liberar)(void* d)){
    fila * f= (fila*)malloc(sizeof(fila));
    if(f == NULL){
        exit(1);
    }
    f->last = f->first = NULL;
    f->n = 0;
    f->liberar = liberar;
    return f;
}

/// @brief Insere um elemento na fila
void filaInserir(fila * f, void* m){
    if(f == NULL){
        exit(1);
    }
    noFila* no = (noFila*)malloc(sizeof(noFila));
    no->prox = NULL;
    no->dado = m;

    if(f->first == NULL){
        f->first = no;
    }else{
        f->last->prox = no;
    }
    f->last = no;
    f->n++;
}

/// @brief Remove primeiro elemento
void* filaRetirar(fila * f){
    if(f->first == NULL){
        exit(1);
    }

    noFila* temp = f->first;
    void* v = temp->dado;

    f->first = f->first->prox;
    if(f->first == NULL){
        f->last = NULL;
    }

    free(temp);
    f->n--;
    return v;
}

/// @brief Remove elemento por índice da fila e retorna o dado.
/// @return ponteiro para dado, ou NULL se índice inválido
void* filaRetirarIndice(fila *f, int indice){
    if (!f || indice < 0 || f->first == NULL)
        return NULL;

    noFila* atual = f->first;
    noFila* anterior = NULL;

    for (int i = 0; i < indice && atual != NULL; i++) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL)
        return NULL;

    if (anterior == NULL) {
        f->first = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    if (atual == f->last) {
        f->last = anterior;
    }

    void* dado = atual->dado;
    free(atual);
    f->n--;

    return dado;
}

/// @brief Libera toda a fila
void filaLiberar(fila ** f){
    if(f == NULL || *f == NULL) return;

    noFila * no = (*f)->first;
    while(no != NULL){
        noFila* temp = no->prox;

        (*f)->liberar(no->dado);
        free(no);

        no = temp;
    }

    free(*f);
    *f = NULL;
}
/// @brief Remove elemento por índice
/// @param f Ponteiro para a fila
/// @param indice Índice do elemento a ser removido
void* filaRemoverIndice(fila* f, int indice) {
    if (!f || f->n == 0 || indice < 0 || indice >= f->n) 
        return NULL;

    noFila* atual = f->first;
    noFila* anterior = NULL;

    for (int i = 0; i < indice; i++) {
        anterior = atual;
        atual = atual->prox;
    }

    if (anterior == NULL)
        f->first = atual->prox;
    else
        anterior->prox = atual->prox;

    if (atual == f->last)
        f->last = anterior;

    void* dado = atual->dado;
    free(atual);
    f->n--;

    return dado;
}