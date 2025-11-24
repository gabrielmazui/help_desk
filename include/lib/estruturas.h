#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H


// forward declarations
typedef struct pilha pilha;
typedef struct fila fila;
typedef struct filaPrioridade filaPrioridade;
typedef struct filaDupla filaDupla;
typedef struct arvoreBinaria arvoreBinaria; 

// struct para guardar os tipos de estruturas usados para guardar os chamados e menus
typedef struct Estruturas{
    pilha* pil;
    fila* chamadosAbertosSemPrioridade;
    filaPrioridade* chamadosAbertosComPrioridade;
    filaDupla* chamadosAndamento;
    filaDupla* chamadosSuspensos;
    fila* chamadosConcluidos;
    arvoreBinaria* estoque;
    arvoreBinaria* atendentes;
}Estruturas;

#endif