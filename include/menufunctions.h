#ifndef MENUFUNCTIONS_H
#define MENUFUNCTIONS_H

#include <time.h>
#include "pilha.h"

// estrutura basica // todos vao ter opcao de voltar e sair 
// todos menus terao suas versoes no struct menuHandler
// os quais serao colocas na pilha, o struct do topo da pilha (o menu), é o que deve ser exibido no instante 
/* ---menu principal---
*   criar chamado -> tem preferencia? -> pergunta prioridade e o nome                                         (2 , 3)
*   visualizar chamados -> tipos de chamados -> editar (funcao separada do handle por conta dessa funcao)     (4, 5)
*   Estatísticas                                                                                              (6)
*   usuarios -> ( se for adm ou mazui ) pode editar                                                           (7)  
*   relatorio -> deletar tudo                                                                                 (8)    
*   logs -> deletar tudo                                                                                      (9)
*   sair                                                                                                      (10)    
*   
*   // tipos 5, 7, 3 nao vao receber as "ordens" do handlerMenuSelect(), vao ter funcoes separadas
*   levando em consideracao que os tipos do menu estao nessa ordem (esquerda para direita)
*
*   tipos 6, 8, 9 vao possuir as info em baixo 
*   tipos 5 , 7 , 8 ( usuarios (adm e normal, tudo junto), chamadas) vao ter info em baixo, porem por funcoes separadas 
*   alem de terem funcoes separadas por conta da opcao editar
*   mainMenu() vai verificar se é algum desses 3 tipos(6,8,9), se for, vai enviar para menuEditHandler(type)
*   onde sera feito o procedimento parecido com o mainMenu(), porem com algumas coisas extras
*
*
*   Menu 1 podera acessar o botao sair pelo ESC (ira abrir o menu tipo 10)
*   no resto, o botao ESC ira ativar o VOLTAR
*   ao sair tera animacao de saida semelhante ao do login
*
*   foi necessario a criacao do menu de tipos 11 e 12 pergunta de sim ou nao se deseja deletar o historico
*   para os relatorios e logs (tipos 8, 9)
*/

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
    int status;
    char titulo[60];
    char descricao[350];
    int prioridade; 
    time_t tempoSimples;
    Tempo tempoComplexo;
    char criador[50];
}chamado;

// status = 1 (aberto)
// status = 2 (em andamento)
// status = 3 (fechado)

void handlerMenuSelect(int type, int selected, pilha** p, char* user);

void menuHandlerEsc(int type, pilha * p);

#endif