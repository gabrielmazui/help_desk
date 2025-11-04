#ifndef MENU_H
#define MENU_H
#include "pilha.h"
#include "fila.h"
#include "filaprioridade.h"
#include "filadupla.h"
#include "utils.h"


// struct que vai conter todos os tipos de menus
typedef struct menuHandler{
    int type; // tipo de menu (id do menu mais ou menos)
    char* titulo;
    char* frase;
    int quant;
    char* options[15];
    void(*funcaoMain)(int, int, char*);
    void(*funcaoExtra)(int);
    int minLinhas; // tamanho minimo de linhas do terminal para esse menu
    int minColunas; // tamanho minimo de colunas do terminal para esse menu
}menuHandler;

extern int lastSelected;
//ultima opcao selecionada no menu anterior

// funcao para limpar a tela
void clear(void);

// menu de login
void login(int codigoErro);

// menu principal
void mainMenu(char* user);

#endif