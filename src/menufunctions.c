#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "menu.h"
#include "pilha.h"
#include "fila.h"
#include "filaprioridade.h"
#include "utils.h"
#include "menufunctions.h"
#include "colors.h"
#include "filadupla.h"
#include "terminal_utils.h"
#include "extra_functions.h"
#include "main_functions.h"

/// @brief função para manipular a lógica de funcao separadas de menus do sistema
/// @param type tipo do menu
/// @param opcao opcao selecionada
/// @param user usuario logado
static void funcaoMainExtra(int type, int opcao, char* user){
    switch(type){
            // criar chamado
        case 3: {
            char t[50] = "";
            char d[350] = "";
            menuCriarChamadoHandler(0, opcao, user, t, d, 0);
            break;
        }   
        case 4:
            // visualizar chamados
            menuVisualizarChamados();
            break;
        case 7:
            // editar usuarios
            break;
        case 14:
            // concluir chamado em andamento
            menuHandlerConcluirChamado();
            break;
        case 18:
            // deletar chamado fechado
            menuHandlerDeletarChamado();
            break;
    }
}

/// @brief função para conteúdo extra dos menus
/// @param type tipo do menu
static void conteudoExtra(int type){
    switch(type){
        case 6:
            // estatisticas
            break;
        case 8:
            // relatorios
            break;
        case 9:
            // logs
            break;
        case 13:
            // atender proximo chamado
            menuAtenderProximoChamado();
            break;
        case 16:
            menuConcluirChamadoConfirmar(lastSelected);
            break;
        case 19:
            // deletar chamado fechado
            menuDeletarChamadoConfirmar(lastSelected);
            break;
    }
}


// vai criar um struct do determinado tipo de menu, e vai retornar ele
menuHandler createMenu(int type){
    // menu 1 sempre estara na pilha, por ser a "raiz" e ter sido criada no menu.c
    // entao nao e necessario que seja criado aqui
    menuHandler menuReturn;
    switch(type){
        case 2: // Criar chamado
            menuReturn = (menuHandler){0};
            menuReturn.type = 2;
            menuReturn.titulo = "Criar chamado";
            menuReturn.frase = "Esse chamado tem prioridade?";
            menuReturn.quant = 4;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";
            menuReturn.options[2] = "Voltar";
            menuReturn.options[3] = "Sair";

            menuReturn.minLinhas = 10;
            menuReturn.minColunas = 45;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;

        case 3: // Função separada, prioridade/nome
            menuReturn = (menuHandler){0};
            menuReturn.type = 3;
            menuReturn.titulo = "";
            menuReturn.frase = "";
            menuReturn.funcaoMain = funcaoMainExtra;
            break;

        case 4: // Função separada, edição dos chamados
            menuReturn = (menuHandler){0};
            menuReturn.type = 4;
            menuReturn.titulo = "";
            menuReturn.frase = "";
            menuReturn.funcaoMain = funcaoMainExtra;
            break;

        case 5: // Estatísticas
            menuReturn = (menuHandler){0};
            menuReturn.type = 5;
            menuReturn.titulo = "Estatisticas";
            menuReturn.frase = "";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Voltar";
            menuReturn.options[1] = "Sair";

            menuReturn.minLinhas = 10;
            menuReturn.minColunas = 25;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = conteudoExtra;
            break;

        case 6: // Função separada, usuários
            menuReturn = (menuHandler){0};
            menuReturn.type = 6;
            menuReturn.titulo = "";
            menuReturn.frase = "";
            menuReturn.funcaoMain = funcaoMainExtra;
            break;

        case 7: // Relatórios
            menuReturn = (menuHandler){0};
            menuReturn.type = 7;
            menuReturn.titulo = "Relatorio das ultimas acoes";
            menuReturn.frase = "";
            menuReturn.quant = 3;
            menuReturn.options[0] = "Deletar todo historico de acoes";
            menuReturn.options[1] = "Voltar";
            menuReturn.options[2] = "Sair";

            menuReturn.minLinhas = 10;
            menuReturn.minColunas = 40;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = conteudoExtra;
            break;

        case 8: // Logs
            menuReturn = (menuHandler){0};
            menuReturn.type = 8;
            menuReturn.titulo = "Logs do sistema";
            menuReturn.frase = "";
            menuReturn.quant = 3;
            menuReturn.options[0] = "Deletar todo historico de logs";
            menuReturn.options[1] = "Voltar";
            menuReturn.options[2] = "Sair";

            menuReturn.minLinhas = 10;
            menuReturn.minColunas = 35;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = conteudoExtra;
            break;

        case 9: // Menu de sair
            menuReturn = (menuHandler){0};
            menuReturn.type = 9;
            menuReturn.titulo = "Sair";
            menuReturn.frase = "Voce deseja mesmo fechar o programa?";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";

            menuReturn.minLinhas = 10;
            menuReturn.minColunas = 50;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;

        case 10: // Deletar todos relatórios
            menuReturn = (menuHandler){0};
            menuReturn.type = 10;
            menuReturn.titulo = "Deletar todos relatorios";
            menuReturn.frase = "Voce deseja mesmo deletar todos os relatorios?";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";

            menuReturn.minLinhas = 10;
            menuReturn.minColunas = 60;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;

        case 11: // Deletar logs
            menuReturn = (menuHandler){0};
            menuReturn.type = 11;
            menuReturn.titulo = "Deletar logs";
            menuReturn.frase = "Voce deseja mesmo deletar todos os logs?";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";

            menuReturn.minLinhas = 10;
            menuReturn.minColunas = 60;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;
        case 12: 
            // aceitar proximo chamado (nao tem chamados abertos)
            menuReturn = (menuHandler){0};
            menuReturn.type = 12;
            menuReturn.titulo = "Aceitar proximo chamado";
            menuReturn.frase = "Nao existem chamados pendentes no momento.";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Voltar";
            menuReturn.options[1] = "Sair";

            menuReturn.minLinhas = 10;
            menuReturn.minColunas = 60;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;
        case 13:
            // atender proximo chamado (tem chamados abertos)
            menuReturn = (menuHandler){0};
            menuReturn.type = 13;
            menuReturn.titulo = "Atender proximo chamado";
            menuReturn.frase = "Deseja atender o proximo chamado da fila?";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";
            menuReturn.minLinhas = 25;
            menuReturn.minColunas = 60;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = conteudoExtra;
            break;
        case 14:
            // concluir chamado em andamento
            menuReturn = (menuHandler){0};
            menuReturn.type = 14;
            menuReturn.titulo = "";
            menuReturn.frase = "";
            menuReturn.funcaoMain = funcaoMainExtra;
            break;
        case 15:
            // concluir chamado em andamente (nao tem chamados em andamento)
            menuReturn = (menuHandler){0};
            menuReturn.type = 15;
            menuReturn.titulo = "Concluir chamado em andamento";
            menuReturn.frase = "Nenhum chamado em andamento disponivel para conclusao.";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Voltar";
            menuReturn.options[1] = "Sair";
            menuReturn.minLinhas = 10;
            menuReturn.minColunas = 60;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;
        case 16:
            // confirmacao de concluir chamado
            menuReturn = (menuHandler){0};
            menuReturn.type = 16;
            menuReturn.titulo = "Concluir chamado em andamento";
            menuReturn.frase = "Tem certeza de que deseja concluir este chamado?";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";
            menuReturn.minLinhas = 25;
            menuReturn.minColunas = 60;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = conteudoExtra;
            break;
        case 17:
            // deletar chamado fechado (nao tem chamados fechados)
            menuReturn = (menuHandler){0};
            menuReturn.type = 17;
            menuReturn.titulo = "Deletar chamado fechado";
            menuReturn.frase = "Nenhum chamado fechado disponivel para deletar.";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Voltar";
            menuReturn.options[1] = "Sair";
            menuReturn.minLinhas = 10;
            menuReturn.minColunas = 60;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;
        case 18:
            // deletar chamado fechado (tem chamados fechados)
            menuReturn = (menuHandler){0};
            menuReturn.type = 18;
            menuReturn.titulo = "";
            menuReturn.frase = "";
            menuReturn.funcaoMain = funcaoMainExtra;
            break;
        case 19:
            // confirmacao de deletar chamado fechado
            menuReturn = (menuHandler){0};
            menuReturn.type = 19;
            menuReturn.titulo = "Deletar chamado fechado";
            menuReturn.frase = "Tem certeza de que deseja deletar este chamado fechado?";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";
            menuReturn.minLinhas = 25;
            menuReturn.minColunas = 60;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = conteudoExtra;
            break;
        case 20:
            // deletar todos os chamados fechados
            menuReturn = (menuHandler){0};
            menuReturn.type = 20;
            menuReturn.titulo = "Deletar todos os chamados fechados";
            menuReturn.frase = "Tem certeza de que deseja deletar todos os chamados fechados?";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";
            menuReturn.minLinhas = 10;
            menuReturn.minColunas = 60;
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;
    }
    return menuReturn;
}

// funcao para esc
void menuHandlerEsc(int type, pilha* p){
    if(type == 1){
        menuHandler mSair = createMenu(9);
        pushPilha(p, mSair);
    }else{
        popPilha(p);
    }
}

// handler para selecionar as opcoes do menu
void handlerMenuSelect(int type, int selected, pilha **p, char* user){
    menuHandler m;
    switch(type){
        case 1:
            switch(selected){
                case 1:
                    // criar chamado
                    m = createMenu(2);
                    pushPilha(*p, m);
                    break;
                case 2:
                    // visualizar chamados
                    m = createMenu(4);
                    pushPilha(*p, m);
                    break;
                case 3:
                    // Estatísticas
                    m = createMenu(5);
                    pushPilha(*p, m);
                    break;
                case 4:
                    // usuarios
                    m = createMenu(6);
                    pushPilha(*p, m);
                    break;
                case 5:
                    // relatorios
                    m = createMenu(7);
                    pushPilha(*p, m);
                    break;
                case 6:
                    // logs
                    m = createMenu(8);
                    pushPilha(*p, m);
                    break;
                case 7:
                    // sair
                    m = createMenu(9);
                    pushPilha(*p, m);
                    break;
            }
            break;
        case 2:
            // criar acao
            switch(selected){
                case 1:
                case 2:
                    // com ou sem preferencia
                    pushPilha(*p, createMenu(3));
                    fflush(stdout);
                    break;
                case 3:
                    // voltar
                    popPilha(*p);
                    break;
                case 4:
                    // sair
                    pushPilha(*p, createMenu(10));
                    break;
                
            }
            break;
        case 5:
            // Estatísticas
            switch(selected){
                // voltar e sair 
                case 1:
                    popPilha(*p);
                    break;
                case 2:
                    pushPilha(*p, createMenu(10));
                    break;
            }
            break;
        case 7:
            // relatorios
            switch(selected){
                case 1:
                    // menu de deletar todos relatorios
                    pushPilha(*p, createMenu(11));
                    break;
                case 2: 
                    // voltar
                    popPilha(*p);
                    break;
                case 3:
                    // sair
                    pushPilha(*p, createMenu(10));
                    break;
            }
            break;
        case 8:
            // logs
            switch(selected){
                case 1:
                    // deletar todos os logs
                    pushPilha(*p, createMenu(12));
                    break;
                case 2:
                    //voltar
                    popPilha(*p);
                    break;
                case 3:
                    // sair
                    pushPilha(*p, createMenu(10));
                    break;
            }
            break;
        case 9:
            // menu de sair
            switch(selected){
                case 1:
                    pilhaLiberar(p);
                    // animacao de sair
                    return;
                case 2:
                    // tira o menu sair do topo da pilha
                    popPilha(*p);
                    break;
            }
            break;
        case 10:
            // menu de deletar todos relatorios
            switch(selected){
                case 1:
                    // deletar todos relatorios
                    // implementar funcao de deletar relatorios
                    popPilha(*p); // tira o menu de deletar relatorios
                    popPilha(*p); // tira o menu de relatorios
                    break;
                case 2:
                    // nao deletar, voltar
                    popPilha(*p); // tira o menu de deletar relatorios
                    break;
            }
            break;
        case 11:
            // menu de deletar todos os logs
            switch(selected){
                case 1:
                    // deletar todos os logs
                    // implementar funcao de deletar logs
                    popPilha(*p); // tira o menu de deletar logs
                    popPilha(*p); // tira o menu de logs
                    break;
                case 2:
                    // nao deletar, voltar
                    popPilha(*p); // tira o menu de deletar logs
                    break;
            }
            break;
        case 12:
            // aceitar proximo chamado (nao tem chamados abertos)
            switch(selected){
                case 1:
                    // voltar
                    popPilha(*p);
                    break;
                case 2:
                    // sair
                    pushPilha(*p, createMenu(9));
                    break;
            }
            break;
        case 13:
            // atender proximo chamado (tem chamados abertos)
            switch(selected){
                case 1:
                    {
                        atenderChamado();
                        popPilha(*p); // tira o menu de atender chamado
                    }
                    break;
                case 2:
                    // nao atender, voltar
                    popPilha(*p); // tira o menu de atender chamado
                    break;
            }
            break;
        case 15:
            // concluir chamado em andamento (nao tem chamados em andamento)
            switch(selected){
                case 1:
                    // voltar
                    popPilha(*p);
                    break;
                case 2:
                    // sair
                    pushPilha(*p, createMenu(9));
                    break;
            }
            break;
        case 16:
            // confirmacao de concluir chamado
            switch(selected){
                case 1:
                    // concluir chamado
                    {
                        concluirChamado(lastSelected);
                        popPilha(*p); // tira o menu de confirmacao
                        popPilha(*p); // tira o menu de concluir chamado
                    }
                    break;
                case 2:
                    // nao concluir, voltar
                    popPilha(*p);
                    break;
            }
            break;
            case 17:
            // deletar chamado fechado (nao tem chamados fechados)
            switch(selected){
                case 1:
                    // voltar
                    popPilha(*p);
                    break;
                case 2:
                    // sair
                    pushPilha(*p, createMenu(9));
                    break;
            }
            break;
            case 19:
            // confirmacao de deletar chamado fechado
            switch(selected){
                case 1:
                    // deletar chamado fechado
                    {
                        deletarChamado(lastSelected, 0);
                        popPilha(*p); // tira o menu de confirmacao
                        popPilha(*p); // tira o menu de deletar chamado
                    }
                    break;
                case 2:
                    // nao deletar, voltar
                    popPilha(*p);
                    break;
            }
            break;
            case 20:
                // confirmacao de deletar todos chamados fechados
                switch(selected){
                    case 1:
                        // deletar todos chamados fechados
                        {
                            deletarChamado(-1, 1);
                            popPilha(*p); // tira o menu de confirmacao
                            popPilha(*p); // tira o menu de deletar chamado
                        }
                        break;
                    case 2:
                        // nao deletar, voltar
                        popPilha(*p);
                        break;
                }
    }
}