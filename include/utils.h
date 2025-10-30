#ifndef UTILS_H
#define UTILS_H

#include "colors.h"
#include "menu.h"
#include "pilha.h"
#include "fila.h"

// struct para guardar os tipos de estruturas usados para guardar os chamados e menus
typedef struct Estruturas{
    pilha* pil;
    fila* filaNormal;
    filaPrioridade* filaPrioridade;
    filaDupla* filadupla;
    // duplamente
}Estruturas;

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


// controlar visibilidade do cursor de texto
// visible -> 1 para mostrar, 0 para esconder
void controlarCursor(int visible);

// repetir caracteres 
// n -> quantidade
// c -> qual caracter
// color -> qual cor (colocar "" caso nao quiser cor)
void repetirChar(int n, char c, char* COLOR);

// comparar duas strings
// retorna 1 se sao iguais
// retorna 0 caso contrario
int compStr(const char a[], const char b[]);

// pega o caractere clicado, sem precisar apertar enter
char getChar(void);

// usa o getChar() e retorna qual tipo de tecla foi pressionada
KeyCode userGetKey(void);

// funcao de input limitado a chars de 1 byte
// aparece na tela enquanto escreve com determinada cor
// tambem cuida se foi pressionado ESC
// tudo que foi escrito vai ir para o buffer
// ele nao vai aceitar mais que a quantidade de maxChars dada como parametro
void inputASCII(char *buffer, int maxChars, const char *color, int * escVerification);

// funcao de input limitado tambem
// apenas para numeros inteiros
// mesmo proposito da funcao anterior
void inputNumeroASCII(char *buffer, int maxChars, const char *color, int *escVerification);

// funcao para inicilar as estruturas
void initEstruturas(Estruturas* estruturas);


//funcao para limpar o terminal
void clear(void);

#endif