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
        case 7:
            // historico
            chamadosFechados();
            break;
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
        case 28:
            // cancelar chamados
            menuCancelarChamados();
            break;
        case 32:
            // estoque
            menuAtualizarEstoque();
            break;
        case 33:
            // adicionar item estoque
            menuAdicionarItemEstoque();
            break;
        case 34:
            // deletar item estoque
            menuDeletarItem();
            break;
    }
}

/// @brief função para conteúdo extra dos menus
/// @param type tipo do menu
static void conteudoExtra(int type){
    switch(type){
        case 4:
            criadorMenu();
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
        case 29:
            // cancelar chamado aberto
            menuCancelarChamadoConfirmar(lastSelected, 1);
            break;
        case 30:
            // cancelar chamado andamento
            menuCancelarChamadoConfirmar(lastSelected, 2);
            break;
        case 31:
            // cancelar chamado suspenso
            menuCancelarChamadoConfirmar(lastSelected, 3);
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
            strcpy(menuReturn->titulo, "Criador");
            strcpy(menuReturn->frase, "");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Voltar";
            menuReturn->options[1] = "Sair";
            menuReturn->minLinhas = 35;
            menuReturn->minColunas = 50;
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
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;

        case 8: // Estoque
            *menuReturn = (menuHandler){0};
            menuReturn->type = 8;
            strcpy(menuReturn->titulo, "Estoque");
            strcpy(menuReturn->frase, "");
            menuReturn->quant = 5;
            menuReturn->options[0] = "Atualizar estoque";
            menuReturn->options[1] = "Adicionar novo item ao estoque";
            menuReturn->options[2] = "Deletar item inutilizados do estoque";
            menuReturn->options[3] = "Voltar";
            menuReturn->options[4] = "Sair";
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
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

        // case 10: // Deletar todo historico de acoes
        //     *menuReturn = (menuHandler){0};
        //     menuReturn->type = 10;
        //     strcpy(menuReturn->titulo, "Deletar todo historico de acoes");
        //     strcpy(menuReturn->frase, "Voce deseja mesmo deletar todo o historico de acoes?");
        //     menuReturn->quant = 2;
        //     menuReturn->options[0] = "Sim";
        //     menuReturn->options[1] = "Nao";

        //     menuReturn->minLinhas = 10;
        //     menuReturn->minColunas = 25;
        //     menuReturn->funcaoMain = NULL;
        //     menuReturn->funcaoExtra = NULL;
        //     break;

        case 11: // Deletar usuario
            *menuReturn = (menuHandler){0};
            menuReturn->type = 11;
            strcpy(menuReturn->titulo, "Deletar usuario");
            strcpy(menuReturn->frase, "Voce deseja mesmo deletar esse usuario?");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Sim";
            menuReturn->options[1] = "Nao";

            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 25;
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
            // cancelar chamado (nao tem chamado para cancelar)
            *menuReturn = (menuHandler){0};
            menuReturn->type = 14;
            strcpy(menuReturn->titulo, "Cancelar chamado");
            strcpy(menuReturn->frase, "Nao existem chamados para cancelar no momento.");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Voltar";
            menuReturn->options[1] = "Sair";

            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;
        case 15:
            // concluir chamado em andamento (nao tem chamados em andamento)
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
            // cancelar chamado (filtro)
            *menuReturn = (menuHandler){0};
            menuReturn->type = 18;
            strcpy(menuReturn->titulo, "Cancelar chamado");
            strcpy(menuReturn->frase, "Qual tipo de chamados deseja cancelar?");
            menuReturn->quant = 5;
            menuReturn->options[0] = "Abertos";
            menuReturn->options[1] = "Andamento";
            menuReturn->options[2] = "Fechados";
            menuReturn->options[3] = "Voltar";
            menuReturn->options[4] = "Sair";
            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
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
        case 25:
            // cancelar chamados (aberto) (nao tem)
           *menuReturn = (menuHandler){0};
            menuReturn->type = 25;
            strcpy(menuReturn->titulo, "Cancelar chamado aberto");
            strcpy(menuReturn->frase, "Nenhum chamado aberto disponivel para cancelar.");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Voltar";
            menuReturn->options[1] = "Sair";
            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;
        case 26:
            // cancelar chamados (andamento) (nao tem)
           *menuReturn = (menuHandler){0};
            menuReturn->type = 26;
            strcpy(menuReturn->titulo, "Cancelar chamado em andamento");
            strcpy(menuReturn->frase, "Nenhum chamado em andamento disponivel para cancelar.");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Voltar";
            menuReturn->options[1] = "Sair";
            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;
        case 27:
            // cancelar chamados (suspenso) (nao tem)
           *menuReturn = (menuHandler){0};
            menuReturn->type = 27;
            strcpy(menuReturn->titulo, "Cancelar chamado suspenso");
            strcpy(menuReturn->frase, "Nenhum chamado suspenso disponivel para cancelar.");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Voltar";
            menuReturn->options[1] = "Sair";
            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;
        case 28:
            // cancelar chamado (tem)
            *menuReturn = (menuHandler){0};
            menuReturn->type = 28;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;
        case 29:
            // confirmar cancelar chamado (aberto)
            *menuReturn = (menuHandler){0};
            menuReturn->type = 29;
            strcpy(menuReturn->titulo, "Cancelar chamado");
            strcpy(menuReturn->frase, "Tem certeza de que deseja cancelar este chamado?");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Sim";
            menuReturn->options[1] = "Nao";
            menuReturn->minLinhas = 25;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = conteudoExtra;
            break;
        case 30:
            // confirmar cancelar chamado (andamento)
            *menuReturn = (menuHandler){0};
            menuReturn->type = 30;
            strcpy(menuReturn->titulo, "Cancelar chamado");
            strcpy(menuReturn->frase, "Tem certeza de que deseja cancelar este chamado?");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Sim";
            menuReturn->options[1] = "Nao";
            menuReturn->minLinhas = 25;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = conteudoExtra;
            break;
        case 31:
            // confirmar cancelar chamado (suspenso)
            *menuReturn = (menuHandler){0};
            menuReturn->type = 31;
            strcpy(menuReturn->titulo, "Cancelar chamado");
            strcpy(menuReturn->frase, "Tem certeza de que deseja cancelar este chamado?");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Sim";
            menuReturn->options[1] = "Nao";
            menuReturn->minLinhas = 25;
            menuReturn->minColunas = 60;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = conteudoExtra;
            break;
        case 32:
            // atualizar estoque
            *menuReturn = (menuHandler){0};
            menuReturn->type = 32;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;
        case 33:
            // adicionar novo item estoque
            *menuReturn = (menuHandler){0};
            menuReturn->type = 33;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;
        case 34:
            // deletar item inutilizado estoque
            *menuReturn = (menuHandler){0};
            menuReturn->type = 34;
            strcpy(menuReturn->titulo, "");
            strcpy(menuReturn->frase, "");
            menuReturn->funcaoMain = funcaoMainExtra;
            break;
        case 35:
            *menuReturn = (menuHandler){0};
            menuReturn->type = 35;
            strcpy(menuReturn->titulo, "Deletar item do estoque");
            strcpy(menuReturn->frase, "Nenhum item disponivel para deletar.");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Voltar";
            menuReturn->options[1] = "Sair";
            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 40;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;
        case 36:
            *menuReturn = (menuHandler){0};
            menuReturn->type = 36;
            strcpy(menuReturn->titulo, "Deletar item do estoque");
            strcpy(menuReturn->frase, "Deseja mesmo deletar este item do estoque?");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Sim";
            menuReturn->options[1] = "Nao";
            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 40;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;
        case 37:
            *menuReturn = (menuHandler){0};
            menuReturn->type = 37;
            strcpy(menuReturn->titulo, "Historico");
            strcpy(menuReturn->frase, "Nao ha acoes no historico.");
            menuReturn->quant = 2;
            menuReturn->options[0] = "Voltar";
            menuReturn->options[1] = "Sair";
            menuReturn->minLinhas = 10;
            menuReturn->minColunas = 40;
            menuReturn->funcaoMain = NULL;
            menuReturn->funcaoExtra = NULL;
            break;
    }
    return menuReturn;
}

// handler para selecionar as opcoes do menu
void handlerMenuSelect(int type, int selected, pilha **p){
    menuHandler* m;
    switch(type){
        case 1:
            switch(usuario->tipo){
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
                        // criador
                        m = createMenu(4);
                        pushPilha(*p, m);
                        break;
                    case 4:
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
                        // criador
                        m = createMenu(4);
                        pushPilha(*p, m);
                        break;
                    case 3:
                        // sair
                        m = createMenu(9);
                        pushPilha(*p, m);
                        break;
                    }
                    break;
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
                        // historico
                        if(estruturasGlobais.chamadosConcluidos->n > 0){
                            m = createMenu(7);
                        } else {
                            m = createMenu(37); // menu de historico vazio
                        }
                        pushPilha(*p, m);
                        break;
                    case 4:
                        // criador
                        m = createMenu(4);
                        pushPilha(*p, m);
                        break;
                    case 5:
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
        
        case 4:
            // criador
            switch(selected){
                // voltar e sair 
                case 1:
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p);
                        free(temp);
                    }
                    break;
                case 2:
                    pushPilha(*p, createMenu(10));
                    break;
            }
            break;
        case 8:
            // estoque
            switch(selected){
                case 1:
                    // atualizar estoque
                    pushPilha(*p, createMenu(32));
                    break;
                case 2:
                    // adicionar novo item ao estoque
                    pushPilha(*p, createMenu(33));
                    break;
                case 3:
                    {
                        // deletar item inutilizados do estoque
                        int q = existeItemDeletavel(estruturasGlobais.estoque); // funcao que verifica se ha itens para deletar
                        if(q == 0){
                            // nao ha itens para deletar
                            pushPilha(*p, createMenu(35));
                        } else {
                            pushPilha(*p, createMenu(34));
                        }
                        break;
                    }
                    
                case 4:
                    //voltar
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                    }
                    break;
                case 5:
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
                    *p = NULL;
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
        case 14:
            // cancelar chamado (nao tem chamado para cancelar)
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
                    pushPilha(*p, createMenu(9));;
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
        case 18:
            // cancelar chamado (filtro)
            switch(selected){
                case 1:
                // abertos
                    if(estruturasGlobais.chamadosAbertosComPrioridade->n > 0 || estruturasGlobais.chamadosAbertosSemPrioridade->n > 0)
                        pushPilha(*p, createMenu(28));
                    else
                        pushPilha(*p, createMenu(25));
                    break;
                case 2:
                // andamento
                    if(estruturasGlobais.chamadosAndamento->n > 0)
                        pushPilha(*p, createMenu(28));
                    else
                        pushPilha(*p, createMenu(26));
                    break;
                case 3:
                // suspensos
                    if(estruturasGlobais.chamadosSuspensos->n > 0)
                        pushPilha(*p, createMenu(28));
                    else
                        pushPilha(*p, createMenu(27));
                    break;
                case 4:
                    // voltar
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                        break;
                    }
                case 5:
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
                        
                    }
                    break;
            }
            break;
        case 25:
            // cancelar chamados (aberto) (nao tem)
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
        case 26:
            // cancelar chamados (andamento) (nao tem)
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
        case 27:
            // cancelar chamados (suspenso) (nao tem)
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
        case 29:
            // confirmar cancelar chamado (aberto)
            switch(selected){
                case 1:
                    // cancelar chamado aberto
                    {
                        cancelarChamado(lastSelected , 1);
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp); // tira o menu de confirmacao
                        menuHandler* temp2 = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp2); // tira o menu de cancelar chamado
                        menuHandler* temp3 = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp3); // tira o menu de cancelar chamado (filtro)
                    }
                    break;
                case 2:
                    // nao cancelar, voltar
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                        break;
                    }
            }
            break;
        case 30:
            // confirmar cancelar chamado (andamento)
            switch(selected){
                case 1:
                    // cancelar chamado andamento
                    {
                        cancelarChamado(lastSelected , 2);
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp); // tira o menu de confirmacao
                        menuHandler* temp2 = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp2); // tira o menu de cancelar chamado
                        menuHandler* temp3 = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp3); // tira o menu de cancelar chamado (filtro)
                    }
                    break;
                case 2:
                    // nao cancelar, voltar
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                        break;
                    }
            }
            break;
        case 31:
            // confirmar cancelar chamado (suspenso)
            switch(selected){
                case 1:
                    // cancelar chamado suspenso
                    {
                        cancelarChamado(lastSelected , 3);
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp); // tira o menu de confirmacao
                        menuHandler* temp2 = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp2); // tira o menu de cancelar chamado
                        menuHandler* temp3 = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp3); // tira the menu de cancelar chamado (filtro)
                    }
                    break;
                case 2:
                    // nao cancelar, voltar
                    {
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp);
                        break;
                    }
            }
            break;
        case 35:
            // deletar item inutilizado estoque
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
        case 36:
            // confirmar deletar item estoque
            switch(selected){
                case 1:
                    // deletar item do estoque
                    {
                        deletarItemChamadoEstoque(lastSelected);
                        menuHandler* temp = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp); // tira o menu de confirmacao
                        menuHandler* temp2 = (menuHandler*)popPilha(*p); // tira o menu de atender chamado
                        free(temp2); // tira o menu de deletar item do estoque
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
        case 37:
            // historico vazio
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
    }
}