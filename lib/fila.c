#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "all.h"

/// @brief Cria uma fila
/// @return Ponteiro para a fila criada
fila * criarFila(void){
    fila * f= (fila*)malloc(sizeof(fila));
    if(f == NULL){
        // log
        exit(1);
    }
    f->last = f->first = NULL;
    f->n = 0;
    return f;
}

/// @brief Insere um elemento na fila
/// @param f Ponteiro para a fila
/// @param m item a ser inserido
void filaInserir(fila * f, void* m){
    if(f == NULL){
        //log
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

/// @brief Remove um elemento da fila
/// @param f Ponteiro para a fila
/// @return item removido
void* filaRetirar(fila * f){
    if(f->first == NULL){
        // fila vazia 
        //log
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


/// @brief Remove um elemento da fila pelo indice
/// @param f Ponteiro para a fila
/// @param indice Indice do elemento a ser removido
/// @return item removido
void* filaRetirarIndice(fila * f, int indice){
    if(f->first == NULL){
        // fila vazia 
        //log
        exit(1);
    }
    noFila* atual = f->first;
    noFila* anterior = NULL;
    int i = 0;
    while(atual != NULL && i < indice){
        anterior = atual;
        atual = atual->prox;
        i++;
    }
    if(atual == NULL){
        // indice invalido
        //log
        exit(1);
    }
    if(anterior == NULL){
        // estamos removendo o primeiro elemento
        f->first = atual->prox;
    }else{
        anterior->prox = atual->prox;
    }
    if(atual == f->last){
        f->last = anterior;
    }
    void* c = atual->dado;
    free(atual);
    f->n--;
    return c;
}

/// @brief Libera a memoria ocupada pela fila
/// @param f Ponteiro duplo para a fila
void filaLiberar(fila ** f){
    noFila * no = (*f)->first;
    while(no != NULL){
        noFila* temp = no->prox;
        free(no->dado);
        free(no);
        no = temp;
    }
    free(*f);
    *f = NULL;
}