#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menufunctions.h"
#include "fila.h"

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
void filaInserir(fila * f, chamado m){
    if(f == NULL){
        //log
        exit(1);
    }
    noFila* no = (noFila*)malloc(sizeof(noFila));
    no->prox = NULL;
    no->chamado = m;
    if(f->first == NULL){
        f->first = no;
    }else{
        f->last->prox = no;
    }
    f->last = no;
    f->n++;
}

chamado filaRetirar(fila * f){
    if(f->first == NULL){
        // fila vazia 
        //log
        exit(1);
    }
    noFila* temp = f->first;
    chamado v = temp->chamado;
    f->first = f->first->prox;
    if(f->first == NULL){
        f->last = NULL;
    }
    free(temp);
    f->n--;
    return v;
}

void filaLiberar(fila ** f){
    noFila * no = (*f)->first;
    while(no != NULL){
        noFila* temp = no->prox;
        free(no);
        no = temp;
    }
    free(*f);
    *f = NULL;
}