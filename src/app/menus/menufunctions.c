#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "all.h"

/// @brief função para manipular a lógica de funcao separadas de menus do sistema
/// @param type tipo do menu
/// @param opcao opcao selecionada
static void funcaoMainExtra(int type, int opcao){
    switch(type){
            // criar chamado
        case 3: {
            char t[50] = "";
            char d[350] = "";
            menuCriarChamadoHandler(0, opcao, t, d, 0);
            break;
        } 
        // case 14:
        //     // concluir chamado em andamento
        //     menuHandlerConcluirChamado();
        //     break;
        // case 18:
        //     // deletar chamado fechado
        //     menuHandlerDeletarChamado();
        //     break;
        case 21:
            // chamados cliente
            clientesChamados();
            break;
        case 22:
            // chamados atendente
            chamadosAtendente();
            break;
        case 23:
            // chamados admin
            chamadosAdmins();
            break;
        case 24:
            // chamados fechados
            chamadosFechados();
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
menuHandler* createMenu(int type){
    // menu 1 sempre estara na pilha, por ser a "raiz" e ter sido criada no menu.c
    // entao nao e necessario que seja criado aqui
    menuHandler* menuReturn = malloc(sizeof(menuHandler));
    switch(type){
        case 2: // Criar chamado
            *menuReturn = (menuHandler){0};
            menuReturn->type = 2;
            strcpy(menuReturn->titulo, "Criar chamado");
            strcpy(menuReturn->frase, "Esse chamado tem prioridade?");
            menuReturn->quant = 4;
            menuReturn->options[0] = "Sim";
            menuReturn->options[1] = "Nao";
            menuReturn->options[2] = "Voltar";
            menuReturn->options[3] = "Sair";

            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 45;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;

        case 3: // Função separada, prioridade/nome
            *menuReturn = (menuHandler){0};
            menuReturn->type = 3;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;

        case 4: // criador
            *menuReturn = (menuHandler){0};
            menuReturn->type = 4;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = conteudoExtra;
            break;

        case 5: // Estatísticas
            *menuReturn = (menuHandler){0};
            menuReturn->type = 5;
            strcpy(menuReturn->titulo, "Estatisticas");
            strcpy(menuReturn->frase, "");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Voltar";
            menuReturn->options[1] = "Sair";

            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 25;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = conteudoExtra;
            break;

        case 6: // Função separada, usuários
            *menuReturn = (menuHandler){0};
            menuReturn->type = 6;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;

        case 7: // Historico
            *menuReturn = (menuHandler){0};
            menuReturn->type = 7;
            strcpy(menuReturn->titulo, "Historico de acoes");
            strcpy(menuReturn->frase, "");
            menuReturn->quant = 3;
            menuReturn->options[0] = "Deletar todo historico de acoes";
            menuReturn->options[1] = "Voltar";
            menuReturn->options[2] = "Sair";

            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 40;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = conteudoExtra;
            break;

        case 8: // Estoque
            *menuReturn = (menuHandler){0};
            menuReturn->type = 8;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;

        case 9: // Menu de sair
            *menuReturn = (menuHandler){0};
            menuReturn->type = 9;
            strcpy(menuReturn->titulo, "Sair");
            strcpy(menuReturn->frase, "Voce deseja mesmo fechar o programa?");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Sim";
            menuReturn->options[1] = "Nao";

            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 50;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;

        case 10: // Deletar todo historico de acoes
            *menuReturn = (menuHandler){0};
            menuReturn->type = 10;
            strcpy(menuReturn->titulo, "Deletar todo historico de acoes");
            strcpy(menuReturn->frase, "Voce deseja mesmo deletar todo o historico de acoes?");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Sim";
            menuReturn->options[1] = "Nao";

            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;

        case 11: // Deletar usuario
            *menuReturn = (menuHandler){0};
            menuReturn->type = 11;
            strcpy(menuReturn->titulo, "Deletar usuario");
            strcpy(menuReturn->frase, "Voce deseja mesmo deletar esse usuarios?");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Sim";
            menuReturn->options[1] = "Nao";

            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;
        case 12: 
            // aceitar proximo chamado (nao tem chamados abertos)
            *menuReturn = (menuHandler){0};
            menuReturn->type = 12;
            strcpy(menuReturn->titulo, "Aceitar proximo chamado");
            strcpy(menuReturn->frase, "Nao existem chamados pendentes no momento.");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Voltar";
            menuReturn->options[1] = "Sair";

            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;
        case 13:
            // atender proximo chamado (tem chamados abertos)
            *menuReturn = (menuHandler){0};
            menuReturn->type = 13;
            strcpy(menuReturn->titulo, "Atender proximo chamado");
            strcpy(menuReturn->frase, "Deseja atender o proximo chamado da fila?");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Sim";
            menuReturn->options[1] = "Nao";
            menuReturn->minLinhas = 25;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = conteudoExtra;
            break;
        case 14:
            // concluir chamado em andamento
            *menuReturn = (menuHandler){0};
            menuReturn->type = 14;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;
        case 15:
            // concluir chamado em andamente (nao tem chamados em andamento)
            *menuReturn = (menuHandler){0};
            menuReturn->type = 15;
            strcpy(menuReturn->titulo, "Concluir chamado em andamento");
            strcpy(menuReturn->frase, "Nenhum chamado em andamento disponivel para conclusao.");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Voltar";
            menuReturn->options[1] = "Sair";
            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;
        case 16:
            // confirmacao de concluir chamado
            *menuReturn = (menuHandler){0};
            menuReturn->type = 16;
            strcpy(menuReturn->titulo, "Concluir chamado em andamento");
            strcpy(menuReturn->frase, "Tem certeza de que deseja concluir este chamado?");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Sim";
            menuReturn->options[1] = "Nao";
            menuReturn->minLinhas = 25;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = conteudoExtra;
            break;
        case 17:
            // deletar chamado fechado (nao tem chamados fechados)
            *menuReturn = (menuHandler){0};
            menuReturn->type = 17;
            strcpy(menuReturn->titulo, "Deletar chamado fechado");
            strcpy(menuReturn->frase, "Nenhum chamado fechado disponivel para deletar.");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Voltar";
            menuReturn->options[1] = "Sair";
            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;
        case 18:
            // deletar chamado fechado (tem chamados fechados)
            *menuReturn = (menuHandler){0};
            menuReturn->type = 18;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;
        case 19:
            // confirmacao de deletar chamado fechado
            *menuReturn = (menuHandler){0};
            menuReturn->type = 19;
            strcpy(menuReturn->titulo, "Deletar chamado fechado");
            strcpy(menuReturn->frase, "Tem certeza de que deseja deletar este chamado fechado?");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Sim";
            menuReturn->options[1] = "Nao";
            menuReturn->minLinhas = 25;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = conteudoExtra;
            break;
        case 20:
            // deletar todos os chamados fechados
            *menuReturn = (menuHandler){0};
            menuReturn->type = 20;
            strcpy(menuReturn->titulo, "Deletar todos os chamados fechados");
            strcpy(menuReturn->frase, "Tem certeza de que deseja deletar todos os chamados fechados?");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Sim";
            menuReturn->options[1] = "Nao";
            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;
        case 21:
            // visualizar chamados cliente
            *menuReturn = (menuHandler){0};
            menuReturn->type = 21;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;
        case 22:
            // visualizar chamados tecnico
            *menuReturn = (menuHandler){0};
            menuReturn->type = 22;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;
        case 23:
            // visualizar chamados admins
            *menuReturn = (menuHandler){0};
            menuReturn->type = 23;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;
        case 24:
            // visualizar chamados fechados
            *menuReturn = (menuHandler){0};
            menuReturn->type = 24;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;
    }
    return menuReturn;
}

// funcao para esc
void menuHandlerEsc(int type, pilha* p){
    if(type == 1){
        menuHandler* mSair = createMenu(9);
        pushPilha(p, mSair);
    }else{
        popPilha(p);
    }
}

// handler para selecionar as opcoes do menu
void handlerMenuSelect(int type, int selected, pilha **p){
    menuHandler* m;
    switch(type){
        case 1:
            switch(usuario.tipo){
                // cliente
                case 1:
                    switch(selected){
                    case 1:
                        // criar chamado
                        m = createMenu(2);
                        pushPilha(*p, m);
                        break;
                    case 2:
                        // visualizar meus chamados
                        m = createMenu(21);
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
                        // historico
                        m = createMenu(7);
                        pushPilha(*p, m);
                        break;
                    case 6:
                        // criador
                        m = createMenu(4);
                        pushPilha(*p, m);
                        break;
                    case 7:
                        // sair
                        m = createMenu(9);
                        pushPilha(*p, m);
                        break;
                }
                break;
                // tecnico
                case 2:
                    switch(selected){
                    case 1:
                        // visualizar meus chamados
                        m = createMenu(22);
                        pushPilha(*p, m);
                        break;
                    case 2:
                        // Estatísticas
                        m = createMenu(5);
                        pushPilha(*p, m);
                        break;
                    case 3:
                        // usuarios
                        m = createMenu(6);
                        pushPilha(*p, m);
                        break;
                    case 4:
                        // historico
                        m = createMenu(7);
                        pushPilha(*p, m);
                        break;
                    case 5:
                        // criador
                        m = createMenu(4);
                        pushPilha(*p, m);
                        break;
                    case 6:
                        // sair
                        m = createMenu(9);
                        pushPilha(*p, m);
                        break;
                    }
                case 3:
                case 4:
                case 5:
                    // gerente ou admin ou criador
                    switch(selected){
                    case 1:
                        // visualizar chamados
                        m = createMenu(23);
                        pushPilha(*p, m);
                        break;
                    case 2:
                        // Estoque
                        m = createMenu(8);
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
                        // historico
                        m = createMenu(7);
                        pushPilha(*p, m);
                        break;
                    case 6:
                        // criador
                        m = createMenu(4);
                        pushPilha(*p, m);
                        break;
                    case 7:
                        // sair
                        m = createMenu(9);
                        pushPilha(*p, m);
                        break;
                    }
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
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                    }
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
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                    }
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
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                    }
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
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                    }
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
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                    }
                    break;
            }
            break;
        case 10:
            // menu de deletar todos relatorios
            switch(selected){
                case 1:
                    // deletar todos relatorios
                    // implementar funcao de deletar relatorios
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp); // tira o menu de deletar relatorios
                        menuHandler* temp2 = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp2); // tira o menu de relatorios
                    }
                    break;
                case 2:
                    // nao deletar, voltar
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp); // tira o menu de deletar relatorios
                    }
                    break;
            }
            break;
        case 11:
            // menu de deletar todos os logs
            switch(selected){
                case 1:
                    // deletar todos os logs
                    // implementar funcao de deletar logs
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp); // tira o menu de deletar logs
                        menuHandler* temp2 = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp2); // tira o menu de logs
                    }
                    break;
                case 2:
                    // nao deletar, voltar
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp); // tira o menu de deletar logs
                    }
                    break;
            }
            break;
        case 12:
            // aceitar proximo chamado (nao tem chamados abertos)
            switch(selected){
                case 1:
                    // voltar
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                    }
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
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp); // tira o menu de atender chamado
                    }
                    break;
                case 2:
                    // nao atender, voltar
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp); // tira o menu de atender chamado
                    }
                    break;
            }
            break;
        case 15:
            // concluir chamado em andamento (nao tem chamados em andamento)
            switch(selected){
                case 1:
                    // voltar
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                    }
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
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp); // tira o menu de confirmacao
                        menuHandler* temp2 = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp2); // tira o menu de concluir chamado
                    }
                    break;
                case 2:
                    // nao concluir, voltar
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                        break;
                    }
            }
            break;
            case 17:
            // deletar chamado fechado (nao tem chamados fechados)
            switch(selected){
                case 1:
                    // voltar
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                        break;
                    }
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
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp); // tira o menu de confirmacao
                        menuHandler* temp2 = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp2); // tira o menu de deletar chamado
                    }
                    break;
                case 2:
                    // nao deletar, voltar
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                        break;
                    }
            }
            break;
            case 20:
                // confirmacao de deletar todos chamados fechados
                switch(selected){
                    case 1:
                        // deletar todos chamados fechados
                        {
                            deletarChamado(-1, 1);
                            menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                            free(temp); // tira o menu de confirmacao
                            menuHandler* temp2 = (menuHandler*)popPilha(*p); // tira o menu de deletar chamado
                            free(temp2); // tira o menu de deletar chamado
                        }
                        break;
                    case 2:
                        // nao deletar, voltar
                        {
                            menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                            free(temp);
                            break;
                        }
                }
    }
}