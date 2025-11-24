#ifndef MENU_HANDLER_H
#define MENU_HANDLER_H

// struct que vai conter todos os tipos de menus
typedef struct menuHandler{
    int type; // tipo de menu (id do menu mais ou menos)
    char titulo[100];
    char frase[200];
    int quant;
    char* options[15];
    void(*funcaoMain)(int, int);
    void(*funcaoExtra)(int);
    int minLinhas; // tamanho minimo de linhas do terminal para esse menu
    int minColunas; // tamanho minimo de colunas do terminal para esse menu
}menuHandler;

#endif
