#ifndef UTILS_H
#define UTILS_H

#include "all.h"

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
KeyCode inputASCII(char *buffer, int maxChars, const char *color, int printarExistente);

// funcao de input limitado tambem
// apenas para numeros inteiros
// mesmo proposito da funcao anterior
KeyCode inputNumeroASCII(char *buffer, int maxChars, const char *color, int printarExistente);

// funcao para atualizar apenas uma opcao do menu
// vai mudar o conteudo de determinada linha
void updateOption(int linha, const char* texto, const char* fundo, const char* cor);

// funcao para quebrar descricao em varias linhas
// texto -> descricao original
// destino -> matriz onde sera salvo o texto quebrado
// retorna quantas linhas foram usadas
int quebrarDescricao(const char *texto, char destino[7][50]);

// criar um chamado e adicionar na fila correta
void criarChamado(chamado* c);

// funcao para atender chamado
void atenderChamado(void);

// funcao para concluir chamado
void concluirChamado(int indice);

// funcao para deletar chamado fechado
void deletarChamado(int indice, int todos);

//funcao para limpar o terminal
void clear(void);

#endif