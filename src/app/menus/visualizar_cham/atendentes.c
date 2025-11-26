#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "all.h"

/// @brief marcar ou desmarcar um chamado no menu de concluir chamado em andamento
/// @param paginaAtual pagina atual
/// @param selected chamado selecionado
/// @param divisaoLinhas divisao de linhas
/// @param marcar 1 para marcar, 0 para desmarcar
static void updateChamadoAndamento(int paginaAtual, int selected, int ** divisaoLinhas, int ** divisaoPaginas, int marcar){
    int linhaInicio = divisaoLinhas[paginaAtual-1][selected - 1];
    if(linhaInicio == 0) linhaInicio = 1; // chamado nao existe
    printf("\033[%d;1H\033[K", linhaInicio); // move para linha e coluna 1 e limpa a linha
    filaDupla* fAndamento = estruturasGlobais.chamadosAndamento;    
    noDuplo *no = fAndamento->inicio;
    chamado* cham;

    for(int i = 0; i < paginaAtual - 1; i++){
        // avancar no noFila
        int countPagina = 0;
        while(divisaoPaginas[i][countPagina] > 0){
            no = no->prox;
            countPagina++;
        }
    }
    for(int i = 1; i < selected; i++){
        no = no->prox;
    }
    cham = (chamado*)no->dado;

    char* color = marcar ? BLUE : YELLOW;
    // printar chamado
    printf("%s+----------------------------------------------------+\n"RESET, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %s%-50s %s|\n"RESET, color, BLUE, (*cham).titulo, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %sPrioridade: %s%-38d %s|\n", color, MAGENTA, WHITE, (*cham).prioridade, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %sCriador: %s%-41s %s|\n"RESET, color, MAGENTA, WHITE, (*cham).criador, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
    color, MAGENTA, WHITE,
    (*cham).tempoComplexo.dia, (*cham).tempoComplexo.mes, (*cham).tempoComplexo.ano,
    (*cham).tempoComplexo.horas, (*cham).tempoComplexo.minutos, (*cham).tempoComplexo.segundos,
    "", color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %-50s %s|\n"RESET, color, "", color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    char descricao[7][50] = {0};
    int linhas = quebrarDescricao((*cham).descricao, descricao);
    // descricao
    for(int i = 0; i < linhas; i++){
        printf("%s| %s%-50s %s|\n"RESET, color, CYAN, descricao[i], color);
        printf("\033[%d;1H\033[K", ++linhaInicio);
    }
    // separador
    printf("%s+----------------------------------------------------+"RESET, color);
    
}

static void copiarUsuarioFila(filaDupla** f){
    filaDupla* filaOrigem = estruturasGlobais.chamadosAndamento;
    noDuplo* no = filaOrigem->inicio;
    while(no != NULL){
        if(strcmp(((chamado*)no->dado)->atendente, usuario.usuario) == 0){
            filaDuplaInserir(*f, 0, no->dado);
        }
        no = no->prox;
    }
}

/// @brief carregar os dados do menu de concluir chamado em andamento
/// @param totalPaginas total de paginas
/// @param divisaoPaginas matriz de divisao de paginas
/// @param divisaoLinhas matriz de divisao de linhas
static filaDupla* carregarDadosMenuConcluirChamado(int * totalPaginas, int ***divisaoPaginas, int *** divisaoLinhas){
    *totalPaginas = 1;
    filaDupla* fAndamento = malloc(sizeof(filaDupla));
    copiarUsuarioFila(&fAndamento);
    noDuplo* no = fAndamento->inicio;
    int count = 0;
    int linhasAtual = 8; // primeira pagina tem header;
    while(no != NULL){
        chamado* cham = (chamado*)no->dado;
        // descricao
        char temp[7][50];
        int linhasDesc = quebrarDescricao((*((chamado*)(cham))).descricao, temp);
        linhasAtual += linhasDesc + 7; // 7 linhas fixas + linhas da desc
        if(linhasAtual > 40){
            (*totalPaginas)++;
            linhasAtual = linhasDesc + 7; // resetar linhasAtual para o novo chamado
        }
        count++;
        no = no->prox;
    }
    
    
    // alocar divisao paginas
    *divisaoPaginas = calloc(sizeof(int*), (*totalPaginas));
    for(int i = 0; i < *totalPaginas; i++){
        (*divisaoPaginas)[i] = calloc(sizeof(int), 10); // impossivel ter 10 chamados por pagina
    }
    // alocar divisao linhas
    *divisaoLinhas = calloc(sizeof(int*), (*totalPaginas));
    for(int i = 0; i < *totalPaginas; i++){
        (*divisaoLinhas)[i] = calloc(sizeof(int), 20); // 10 * 2 maximo de limites dos chamados por pagina
    }
    // preencher divisao paginas e linhas
    no = fAndamento->inicio;
    int paginaAtual = 0;
    int chamadoAtualNaPagina = 0;
    linhasAtual = 8; // primeira pagina tem header
    (*divisaoLinhas)[0][0] = 9; // primeira linha do primeiro chamado
    while(no != NULL){
        chamado* cham = (chamado*)(no->dado);
        // descricao
        char temp[7][50];
        int linhasDesc = quebrarDescricao((*((chamado*)(cham))).descricao, temp);
        linhasAtual += linhasDesc + 7; // 7 linhas fixas + linhas da desc
        if(linhasAtual > 40){
            // nova pagina
            paginaAtual++;
            linhasAtual = linhasDesc + 7; // resetar linhasAtual para o novo chamado
            chamadoAtualNaPagina = 0;
        }
        
        // preencher divisao paginas e linhas
        (*divisaoPaginas)[paginaAtual][chamadoAtualNaPagina] = (count > 0) ? 1 : 0; // marcar que tem chamado ou que nao tem
        (*divisaoLinhas)[paginaAtual][chamadoAtualNaPagina + 1] = linhasAtual + 1; // marcar linha inicial do chamado
        chamadoAtualNaPagina++;
        no = no->prox;
    }
    return fAndamento;
}

/// @brief printar o menu de concluir chamado em andamento 
/// @param divisaoPaginas matriz de divisao de paginas
/// @param totalPaginas total de paginas
/// @param paginaAtual pagina atual
/// @param selected chamado ou opcao selecionada
static void printarMenuConcluirChamado(int ** divisaoPaginas, int totalPaginas, int paginaAtual, int selected, filaDupla* fAndamento){
    noDuplo* no = fAndamento->inicio;
    int count = 0;
    if(paginaAtual == 1){
        // primeira pagina tem 13 linhas a menos (header)
        char* titulo = "Concluir Chamado em Andamento";
        repetirChar(15, '-', BLUE);
        printf(BLUE"%s"RESET, titulo);
        repetirChar(15, '-', BLUE);
        printf("\n");
        printf(GREEN" %s"RESET, "Selecione um chamado para concluir");
        printf("\n\n");

        /*
        * Voltar - 1 
        * Sair - 2
        */

        char* corTextoNormal = BLUE;
        char* corTextoSair = RED;
        char* fundoSelecaoNormal = BG_BLUE;
        char* fundoSelecaoSair = BG_RED;

        // printar opcoes
        char* opcoes[2] = {
            "Voltar",
            "Sair"
        };
        for(int i = 1; i <= 2; i++){
            if(i == selected){
                if(i == 2){
                    // sair
                    printf(" %s%s\n"RESET, fundoSelecaoSair, opcoes[i-1]);
                } else {
                    printf(" %s%s\n"RESET, fundoSelecaoNormal, opcoes[i-1]);
                }
            } else {
                if(i == 2){
                    // sair
                    printf(" %s%s\n"RESET, corTextoSair, opcoes[i-1]);
                } else {
                    printf(" %s%s\n"RESET, corTextoNormal, opcoes[i-1]);
                }
            }
        }
        printf("\n");
        int tamanhoStr = strlen(titulo);
        repetirChar(30+tamanhoStr, '-', BLUE);
        printf("\n\n");
    }
    for(int i = 0; i < paginaAtual -1; i++){
        // avancar no noFila
        int countPagina = 0;
        while(divisaoPaginas[i][countPagina] > 0){
            no = no->prox;
            countPagina++;
        }
    }
    
    while(divisaoPaginas[paginaAtual-1][count] > 0){
        chamado* cham = (chamado*)(no->dado);
        char * color = YELLOW;
        if((selected == count + 3 && paginaAtual == 1) || (selected == count +1 && paginaAtual > 1)){
            // chamado selecionado
            color = BLUE;
        }
        printf("%s+----------------------------------------------------+\n"RESET, color);
        printf("%s| %s%-50s %s|\n"RESET, color, BLUE, (*((chamado*)(cham))).titulo, color);
        
        // prioridade
        printf("%s| %sPrioridade: %s%-38d %s|\n", color, MAGENTA, WHITE, (*((chamado*)(cham))).prioridade, color);
        
        // criador
        printf("%s| %sCriador: %s%-41s %s|\n"RESET, color, MAGENTA, WHITE, (*((chamado*)(cham))).criador, color);

        // data e hora
        printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
        color, MAGENTA, WHITE,
        (*((chamado*)(cham))).tempoComplexo.dia, (*((chamado*)(cham))).tempoComplexo.mes, (*((chamado*)(cham))).tempoComplexo.ano,
        (*((chamado*)(cham))).tempoComplexo.horas, (*((chamado*)(cham))).tempoComplexo.minutos, (*((chamado*)(cham))).tempoComplexo.segundos,
        "", color);
        
        // linha vazia
        printf("%s| %-50s %s|\n"RESET, color, "", color);
        // descricao
        char descricao[7][50] = {0};
        int linhas = quebrarDescricao((*((chamado*)(cham))).descricao, descricao);
        // descricao
        for(int i = 0; i < linhas; i++){
            printf("%s| %s%-50s %s|\n"RESET, color, CYAN, descricao[i], color);
        }
        // separador
        printf("%s+----------------------------------------------------+\n"RESET, color);
        count++;
        no = no->prox;
    }
    
    printf("\033[42;1H"); // ir pra linha 42 coluna 1
    printf(BLUE" Pagina %d/%d"RESET, paginaAtual, totalPaginas);
}

/// @brief função para manipular o menu de concluir chamado em andamento
void chamadosAtendente(void){
    esperar_tamanho_minimo(42, 60);
    if(terminalPequenoAlertado){   
        terminalPequenoAlertado = 0;
    }
    clear();
    int totalPaginas = 0;
    int ** divisaoPaginas = NULL;
    int ** divisaoLinhas = NULL;
    filaDupla* fAndamento = carregarDadosMenuConcluirChamado(&totalPaginas, &divisaoPaginas, &divisaoLinhas);
    int paginaAtual = 1;
    int selected = 1;

    printarMenuConcluirChamado(divisaoPaginas, totalPaginas, paginaAtual, selected, fAndamento);
    while(1){
        int ultimaOpcao = 0;
        if(paginaAtual == 1){
            // primeira pagina tem 2 opcoes extras (voltar e sair)
            ultimaOpcao +=2;
        }
        for(int i = 0; divisaoPaginas[paginaAtual-1][i] > 0; i++){
            ultimaOpcao++;
        }
        KeyCode k = userGetKey();
        if(k == RESIZE_EVENT){
            esperar_tamanho_minimo(42, 60);
            if(terminalPequenoAlertado){
                clear();
                printarMenuConcluirChamado(divisaoPaginas, totalPaginas, paginaAtual, selected, fAndamento);
                terminalPequenoAlertado = 0;
            }
        }else if(k == KC_ESC){
            menuHandler* temp = (menuHandler*)popPilha(estruturasGlobais.pil); // tira o menu de atender chamado
            free(temp); // tira o menu de atender chamado
            break;
        }else if(k == KC_UP){
            if(selected == 2 && paginaAtual == 1){
                // desmarcar sair
                updateOption(5, "Sair", "", RED);
                // marcar voltar
                updateOption(4, "Voltar", BG_BLUE, "");
                selected--;
            }else if(selected == 3 && paginaAtual == 1 && ultimaOpcao > 2){
                // desmarcar chamado
                updateChamadoAndamento(paginaAtual, selected - 2, divisaoLinhas, divisaoPaginas, 0);
                selected--;
                // marcar sair
                updateOption(5, "Sair", BG_RED, "");
            }else{
                if(selected == 1 && paginaAtual > 1){
                    // ir para pagina anterior
                    paginaAtual--;
                    // selecionar ultima opcao da pagina anterior
                    int ultimaOpcaoTemp = 0;
                    for(int i = 0; divisaoPaginas[paginaAtual-1][i] > 0; i++){
                        ultimaOpcaoTemp++;
                    }
                    if(paginaAtual == 1){
                        ultimaOpcaoTemp += 2; // ajustar para voltar e sair
                    }
                    selected = ultimaOpcaoTemp;
                    clear();
                    printarMenuConcluirChamado(divisaoPaginas, totalPaginas, paginaAtual, selected, fAndamento);
                }else if(selected > 1){
                    int selectedPrev = selected;
                    if(paginaAtual == 1){
                        selectedPrev -= 2; // ajustar para pular opcoes voltar e sair
                    }
                    updateChamadoAndamento(paginaAtual, selectedPrev, divisaoLinhas, divisaoPaginas, 0);
                    selected--;
                    selectedPrev--;
                    updateChamadoAndamento(paginaAtual, selectedPrev, divisaoLinhas, divisaoPaginas, 1);
                }
            }
        }else if(k == KC_DOWN){
            if(selected == 1 && paginaAtual == 1){
                // desmarcar voltar
                updateOption(4, "Voltar", "", BLUE);
                // marcar sair
                updateOption(5, "Sair", BG_RED, "");
                selected++;
            }else if(selected == 2 && paginaAtual == 1 && ultimaOpcao > 2){
                // desmarcar sair
                updateOption(5, "Sair", "", RED);
                selected++;
                updateChamadoAndamento(paginaAtual, selected - 2, divisaoLinhas, divisaoPaginas, 1);
            }else{
                if(selected == ultimaOpcao && paginaAtual < totalPaginas){
                    selected = 1;
                    paginaAtual++;
                    clear();
                    printarMenuConcluirChamado(divisaoPaginas, totalPaginas, paginaAtual, selected, fAndamento);
                }else if(selected < ultimaOpcao){
                    int selectedPrev = selected;
                    if(paginaAtual == 1){
                        selectedPrev -= 2; // ajustar para pular opcoes voltar e sair
                    }
                    updateChamadoAndamento(paginaAtual, selectedPrev, divisaoLinhas, divisaoPaginas, 0);
                    selected++;
                    selectedPrev++;
                    updateChamadoAndamento(paginaAtual, selectedPrev, divisaoLinhas, divisaoPaginas, 1);
                }
            }
        }else if(k == KC_RIGHT){
            if(paginaAtual < totalPaginas){
                paginaAtual++;
                selected = 1;
                clear();
                printarMenuConcluirChamado(divisaoPaginas, totalPaginas, paginaAtual, selected, fAndamento);
            }
        }else if(k == KC_LEFT){
            if(paginaAtual > 1){
                paginaAtual--;
                selected = 1;
                clear();
                printarMenuConcluirChamado(divisaoPaginas, totalPaginas, paginaAtual, selected, fAndamento);
            }
        }else if(k == KC_ENTER){
            if(paginaAtual == 1 && selected == 1){
                // voltar
                menuHandler* temp = (menuHandler*)popPilha(estruturasGlobais.pil); // tira o menu de atender chamado
                free(temp); // tira o menu de atender chamado
                lastSelected = 1;
                break;
            }else if(paginaAtual == 1 && selected == 2){
                // sair
                menuHandler* m = createMenu(9);
                pushPilha(estruturasGlobais.pil, m);
                lastSelected = 2;
                // apenas por seguranca (isso nao vai ser usado)
                break;
            }else{
                int countTotal = 0; // qual chamado foi escolhido (numero absoluto na fila)
                for(int i = 0; i < paginaAtual -1; i++){
                    int countPagina = 0;
                    while(divisaoPaginas[i][countPagina] > 0){
                        countTotal++;
                        countPagina++;
                    }
                }
                if(paginaAtual == 1){
                    countTotal += selected - 2; // ajustar para pular opcoes voltar e sair
                } else {
                    countTotal += selected;
                }
                // concluir chamado selecionado
                menuHandler* m = createMenu(16);
                pushPilha(estruturasGlobais.pil, m);
                lastSelected = (countTotal - 1); // ajustar para indice 0
                break;
            }
        }
    }
    for(int i = 0; i < totalPaginas; i++){
        free(divisaoPaginas[i]);
        free(divisaoLinhas[i]);
    }
    free(divisaoPaginas);
    free(divisaoLinhas);
    
    noDuplo* no = fAndamento->inicio;
    while(no != NULL){
        noDuplo* temp = no;
        no = no->prox;
        free(temp);
    }
    free(fAndamento);
}