#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifndef _WIN32
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>


#else
#include <conio.h>
#include <windows.h>
#endif

#include "all.h"

// funcao para mostrar pos animacao de carregamento
static void bemVindo(void){
    repetirChar(35,'=', CYAN);
    printf(GREEN"BEM-VINDO"RESET);
    repetirChar(35,'=', CYAN);
    printf("\n");
    printf(CYAN
    " __    __  ________  __        _______         _______   ________   ______   __    __ \n"
    "|  \\  |  \\|        \\|  \\      |       \\       |       \\ |        \\ /      \\ |  \\  /  \\\n"
    "| $$  | $$| $$$$$$$$| $$      | $$$$$$$\\      | $$$$$$$\\| $$$$$$$$|  $$$$$$\\| $$ /  $$\n"
    "| $$__| $$| $$__    | $$      | $$__/ $$      | $$  | $$| $$__    | $$___\\$$| $$/  $$ \n"
    "| $$    $$| $$  \\   | $$      | $$    $$      | $$  | $$| $$  \\    \\$$    \\ | $$  $$  \n"
    "| $$$$$$$$| $$$$$   | $$      | $$$$$$$       | $$  | $$| $$$$$    _\\$$$$$$\\| $$$$$\\  \n"
    "| $$  | $$| $$_____ | $$_____ | $$            | $$__/ $$| $$_____ |  \\__| $$| $$ \\$$\\ \n"
    "| $$  | $$| $$     \\| $$     \\| $$            | $$    $$| $$     \\ \\$$    $$| $$  \\$$\\\n"
    " \\$$   \\$$ \\$$$$$$$$ \\$$$$$$$$ \\$$             \\$$$$$$$  \\$$$$$$$$  \\$$$$$$  \\$$   \\$$\n"
    RESET);
    printf("\n");
    repetirChar(79,'=', CYAN);
    printf("\n\n");
    repetirChar(20, ' ', "");
    printf(CYAN"Seu %s%sMELHOR%s%s sistema de ajuda automatica\n"RESET, RESET,GREEN,RESET,CYAN);

    repetirChar(15, ' ', "");
    printf(CYAN"Sempre disponivel para voce a %s%sQUALQUER%s%s momento\n"RESET,RESET,GREEN,RESET,CYAN);
    printf("\n");
    repetirChar(79,'=',CYAN);
    printf("\n");
}

// funcao para desenhar o menu atual na tela
static void desenharMenuAtual(menuHandler* m, int selected){
    
    if(m->type == 1){
        bemVindo();
        printf("\n");
        fflush(stdout);
    }
    
    repetirChar(15, '-', BLUE);
    printf(BLUE"%s"RESET, m->titulo);
    repetirChar(15, '-', BLUE);
    printf("\n");
    
    int tamanhoFrase = strlen(m->frase);
    if(tamanhoFrase > 0){
        printf(GREEN" %s"RESET, m->frase);
        printf("\n\n");
        fflush(stdout);
    }else{
        printf("\n");
    }
    
    for(int i = 0; i < m->quant; i++){
        if(m->type == 10){
            char* fundoSair = BG_BLUE;
            char* textoSair = BLUE;
            // no caso do menu sair
            // a primeira opcao (sim) sera o botao vermelho
            if(i == 0){
                fundoSair = BG_RED;
                textoSair = RED;
            }
            if(selected == i+1){
                printf(" %s%s"RESET,fundoSair ,m->options[i]);
            }else{
                printf(" %s%s"RESET,textoSair ,m->options[i]);
            }
            printf("\n");
            continue;
            // ignora as proximas linhas pois elas nao servem para esse caso
        }
        char* corTxt = BLUE;
        char* fundoTxt = BG_BLUE;
        if(i == m->quant - 1 && m->type != 10){
            // se for a ultima opcao (sair) deixar vermelho
            // se nao for o menu de sair (tipo == 10)
            corTxt = RED;
            fundoTxt = BG_RED;
        }
        if(selected == i+1){
            printf(" %s%s"RESET,fundoTxt, m->options[i]);
        }else {
            printf(" %s%s"RESET,corTxt, m->options[i]);
        }
        printf("\n");
        fflush(stdout);
        
    }
    printf("\n");
    int tamanhoStr = strlen(m->titulo);
    repetirChar(30+tamanhoStr, '-', BLUE);
    fflush(stdout);
    
    if(m->funcaoExtra != NULL){
        // se o menu tiver uma funcao extra, mais coisa para printar
        // vai printar a baixo do menu em si, em baixo das opcoes
        printf("\n\n");
        m->funcaoExtra(m->type);
    }
}

// conta quantas linhas tem ate a 1 opcao do menu
static int contadorDeLinhas(menuHandler* m){
    int q = 4;
    if(m->type == 1){
        q += 18;
    }
    if(strlen(m->frase) > 0){
        q++;
    }
    return q;
}

// funcao handle para os menus, vai ficar no loop infinito, lendo as teclas
// sempre mostrando o topo da pilha
void mainMenu(void){
    menuHandler* menu1 = malloc(sizeof(menuHandler));
    menu1->type = 1;
    strcpy(menu1->titulo, "Menu principal");
    strcpy(menu1->frase, "");
    menu1->minColunas = 80;
    menu1->funcaoMain = NULL;
    menu1->funcaoExtra = NULL;
    char* options1[] = {"Criar chamado", "Visualizar os meus chamados", "Estatisticas", "Historico", "Usuarios", "Criador", "Sair"};
    char* options2[] = {"Visualizar os meus chamados", "Estatisticas", "Historico", "Usuarios", "Criador", "Sair"};
    char* options3[] = {"Visualizar chamados", "Estoque", "Estatisticas", "Historico", "Usuarios", "Criador", "Sair"};
    char* options4[] = {"Visualizar chamados", "Estoque", "Estatisticas", "Historico", "Usuarios", "Criador", "Sair"};
    char* options5[] = {"Visualizar chamados", "Estoque", "Estatisticas", "Historico", "Usuarios", "Criador", "Sair"};
    switch(usuario.tipo){
        case 1:
            // cliente
            menu1->quant = 7;
            for(int i = 0; i < menu1->quant; i++){
                menu1->options[i] = options1[i];
            }
            break;
        case 2:
            // tecnico
            menu1->quant = 6;
            for(int i = 0; i < menu1->quant; i++){
                menu1->options[i] = options2[i];
            }
            break;
        case 3:
            // gerente
            menu1->quant = 7;
            for(int i = 0; i < menu1->quant; i++){
                menu1->options[i] = options3[i];
            }
            break;
        case 4:
            // admin
            menu1->quant = 7;
            for(int i = 0; i < menu1->quant; i++){
                menu1->options[i] = options4[i];
            }
            break;
        case 5:
            // criador
            menu1->quant = 7;
            for(int i = 0; i < menu1->quant; i++){
                menu1->options[i] = options5[i];
            }
            break;
    }
    
    menu1->minLinhas = 30;
    
    // colocar no top da fila
    initConfigs();
    pilha * p = estruturasGlobais.pil;
    pushPilha(p, menu1);
    // colocar menu1 no topo
    
    int selected = 1; // opcao selecionada no momento
    
    menuHandler* menuEscolhido;
    menuEscolhido = (menuHandler*)topoPilha(p);
    // ja verifica se precisa redimensionar a tela antes de mostrar o menu
    
    desenharMenuAtual(menuEscolhido, selected);
    // loop que vai ler as teclas, e mudar os menus caso for necessario
    while(p != NULL){

        // se o menu escolhido tiver uma funcao externa
        // chame ela e va para a proxima iteracao
        
        if(menuEscolhido->funcaoMain != NULL){
            menuEscolhido->funcaoMain(menuEscolhido->type, lastSelected);
            // depois da funcao separada, verificar novamente o topo da pilha
            selected = 1;
            clear();
            menuEscolhido = (menuHandler*)topoPilha(p);
            desenharMenuAtual(menuEscolhido, selected);
            continue;
        }

        esperar_tamanho_minimo(menuEscolhido->minLinhas, menuEscolhido->minColunas);
        if(terminalPequenoAlertado){
            terminalPequenoAlertado = 0;
            clear();
            desenharMenuAtual(menuEscolhido, selected);
        }
    
        KeyCode tecla = userGetKey();
        if(tecla == RESIZE_EVENT){
            esperar_tamanho_minimo(menuEscolhido->minLinhas, menuEscolhido->minColunas);
            if(terminalPequenoAlertado){
                terminalPequenoAlertado = 0;
                clear();
                desenharMenuAtual(menuEscolhido, selected);
            }
            continue;

        }else if(tecla == KC_ENTER){
            // chamar funcao com o tipo de menu e com o selected , do outro arquivo
            handlerMenuSelect(menuEscolhido->type, selected, &p);
            if(p == NULL){
                // se a pilha estiver vazia, sair do loop
                break;
            }
            lastSelected = selected;
            selected = 1;
            clear();
            menuEscolhido = (menuHandler*)topoPilha(p);
            if(menuEscolhido->funcaoMain != NULL){
                // se a funcao tiver funcao main, chame ela e continue
                // previnir o desenho do menu
                continue;
            }
            desenharMenuAtual(menuEscolhido, selected);
            // a partir de agora, na proxima iteracao ele vai ler o struct do topo da pilha
        }else if(tecla == KC_UP){
            if(selected > 1){
                int linhas = contadorDeLinhas(menuEscolhido); // quantas linhas tem ate a primeira opcao
                
                char* corTexto1 = BLUE;
                char* fundo1 = "";
                char* corTexto2 = "";
                char* fundo2 = BG_BLUE;
                
                if(menuEscolhido->type == 10 || selected == menuEscolhido->quant){
                    // caso ele esteja saindo da opcao SIM (do menu sair)
                    // ou se o selecionado for a ultima opcao (opcao sair)
                    // a qual sempre e vermelha
                    corTexto1 = RED;    
                }
                
                
                // atualizar a linha que estava selecionada antes
                updateOption(linhas + selected - 2, menuEscolhido->options[selected-1], fundo1, corTexto1);
                selected--;
                
                // atualizar a opcao selecionada agora
                updateOption(linhas + selected - 2, menuEscolhido->options[selected-1], fundo2, corTexto2);
                
            }
        }else if(tecla == KC_DOWN){
            if(selected < menuEscolhido->quant){
                int linhas = contadorDeLinhas(menuEscolhido); // quantas linhas tem ate a primeira opcao
                
                char* corTexto1 = BLUE;
                char* fundo1 = "";
                char* corTexto2 = "";
                char* fundo2 = BG_BLUE;
                
                if(menuEscolhido->type == 10 || selected == menuEscolhido->quant-1){
                    // indo sempre do penultimo para o ultimo nesse caso
                    // sempre sera a ultima opcao a opcao sim do menu sair
                    // ou a opcao sair de todos os menus
                    fundo2 = BG_RED;    
                }
                
                // atualizar a linha que estava selecionada antes
                updateOption(linhas + selected - 2, menuEscolhido->options[selected-1], fundo1, corTexto1);
                selected++;
                
                // atualizar a opcao selecionada agora
                updateOption(linhas + selected - 2, menuEscolhido->options[selected-1], fundo2, corTexto2);
            }
        }else if(tecla == KC_ESC){
            // chama funcao de voltar 
            // no caso do menu UM sera de sair
            if(menuEscolhido->type == 1){
                menuHandler *m = createMenu(9);
                pushPilha(p, m);
                selected = 1;
                clear();
                menuEscolhido = (menuHandler*)topoPilha(p);
                desenharMenuAtual(menuEscolhido, selected);
            }else{
                popPilha(p);
                clear();
                menuEscolhido = (menuHandler*)topoPilha(p);
                desenharMenuAtual(menuEscolhido, selected);
            }
        }
    }
   
    filaPrioridadeLiberar(&estruturasGlobais.chamadosAbertosComPrioridade);
    filaLiberar(&estruturasGlobais.chamadosAbertosSemPrioridade);
    filaDuplaLiberar(&estruturasGlobais.chamadosAndamento);
    filaDuplaLiberar(&estruturasGlobais.chamadosSuspensos);
    filaLiberar(&estruturasGlobais.chamadosConcluidos);
    arv_liberar(&estruturasGlobais.estoque);
    arv_liberar(&estruturasGlobais.atendentes);
}