#ifndef ITEMS_H
#define ITEMS_H

typedef struct Item {
    int id;
    char nome[45];
    int quantidade;
    int requisicoes;
} Item;

#endif