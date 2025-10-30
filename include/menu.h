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
    void(*funcaoMain)(int, int, Estruturas, char*);
    void(*funcaoExtra)(int, Estruturas);
}menuHandler;

// funcao que retorna Keycode (seta pra cima pra baixo, enter, esc)
KeyCode userGetKey(void);

// funcao para ler um char por vez
char getChar(void);

// funcao para limpar a tela
void clear(void);

// menu de login
void login(int codigoErro);

// menu principal
void mainMenu(char* user);

#endif