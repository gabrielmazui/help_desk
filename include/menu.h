#ifndef MENU_H
#define MENU_H
#include "pilha.h"
#include "fila.h"
#include "filaprioridade.h"
#include "filadupla.h"

// struct para guardar os tipos de estruturas usados para guardar os chamados e menus
typedef struct Estruturas{
    pilha* pil;
    fila* filaNormal;
    filaPrioridade* filaPrioridade;
    filaDupla* filadupla;
    // duplamente
}Estruturas;

// struct que vai conter todos os tipos de menus
typedef struct menuHandler{
    int type; // tipo de menu (id do menu mais ou menos)
    char* titulo;
    char* frase;
    int quant;
    char* options[15];
    void(*funcaoMain)(int, Estruturas, char*);
    void(*funcaoExtra)(int, Estruturas);
}menuHandler;

// tipo de tecla clicada
typedef enum {
    KC_OTHER = 0,
    KC_UP,
    KC_DOWN,
    KC_LEFT,
    KC_RIGHT,
    KC_ENTER,
    KC_ESC
} KeyCode;

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