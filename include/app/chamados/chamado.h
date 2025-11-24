#ifndef CHAMADO_H
#define CHAMADO_H

#include <time.h>

typedef struct fila fila;// forward declaration

// struct personalizado de tempo
typedef struct Tempo{
    int segundos;
    int minutos;
    int horas;
    int dia;
    int mes;
    int ano;
}Tempo;

// struct para chamados
typedef struct chamado{
    int id;
    char titulo[60];
    char descricao[350];
    int prioridade; 
    time_t tempoSimples;
    Tempo tempoComplexo;
    char criador[50];
    char atendente[50];
    fila * materiais;
}chamado;

#endif