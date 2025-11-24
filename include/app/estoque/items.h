#ifndef ITEMS_H
#define ITEMS_H

typedef struct Item {
    int id;
    char nome[50];
    int quantidade;
    int requisicoes;
} Item;

#endif