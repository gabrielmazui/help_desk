#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "all.h"

/// @brief marcar ou desmarcar um chamado no menu de deletar chamado fechado
/// @param paginaAtual pagina atual
/// @param selected chamado selecionado
/// @param divisaoLinhas divisao de linhas
/// @param marcar 1 para marcar, 0 para desmarcar
static void updateChamadoFechado(int paginaAtual, int selected, int ** divisaoLinhas, int ** divisaoPaginas, int marcar){
    int linhaInicio = divisaoLinhas[paginaAtual-1][selected - 1];
    if(linhaInicio == 0) linhaInicio = 1; // chamado nao existe
    printf("\033[%d;1H\033[K", linhaInicio); // move para linha e coluna 1 e limpa a linha
    fila* fd = estruturasGlobais.chamadosConcluidos;
    noFila *no = fd->first;
    chamado * cham;

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
    char* color = marcar ? BLUE : RED;
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

/// @brief carregar os dados do menu de concluir chamado em andamento
/// @param totalPaginas total de paginas
/// @param divisaoPaginas matriz de divisao de paginas
/// @param divisaoLinhas matriz de divisao de linhas
static void carregarDadosDeletarChamado(int * totalPaginas, int ***divisaoPaginas, int *** divisaoLinhas){
    *totalPaginas = 1;
    fila* fd = estruturasGlobais.chamadosConcluidos;
    noFila* no = fd->first;
    int count = 0;
    int linhasAtual = 9; // primeira pagina tem header;
    while(no != NULL){
        chamado* cham = (chamado*)(no->dado);
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
    no = fd->first;
    int paginaAtual = 0;
    int chamadoAtualNaPagina = 0;
    linhasAtual = 9; // primeira pagina tem header
    (*divisaoLinhas)[0][0] = 10; // primeira linha do primeiro chamado
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
        (*divisaoPaginas)[paginaAtual][chamadoAtualNaPagina] = (count > 0) ? 1 : 0; // marcar que tem chamado
        (*divisaoLinhas)[paginaAtual][chamadoAtualNaPagina + 1] = linhasAtual + 1; // marcar linha inicial do chamado
        chamadoAtualNaPagina++;
        no = no->prox;
    }
}

/// @brief printar o menu de deletar chamado fechado 
/// @param divisaoPaginas matriz de divisao de paginas
/// @param totalPaginas total de paginas
/// @param paginaAtual pagina atual
/// @param selected chamado ou opcao selecionada
static void printarMenuDeletarChamado(int ** divisaoPaginas, int totalPaginas, int paginaAtual, int selected){
    fila* fd = estruturasGlobais.chamadosConcluidos;
    noFila* no = fd->first;
    int count = 0;
    if(paginaAtual == 1){
        char* titulo = "Concluir Chamado em Andamento";
        repetirChar(15, '-', BLUE);
        printf(BLUE"%s"RESET, titulo);
        repetirChar(15, '-', BLUE);
        printf("\n");
        printf(GREEN" %s"RESET, "Selecione um chamado para deletar");
        printf("\n\n");


        char* corTextoNormal = BLUE;
        char* corTextoSair = RED;
        char* fundoSelecaoNormal = BG_BLUE;
        char* fundoSelecaoSair = BG_RED;

        // printar opcoes
        char* opcoes[3] = {
            "Deletar todos os chamados fechados",
            "Voltar",
            "Sair"
        };
        for(int i = 1; i <= 3; i++){
            if(i == selected){
                if(i == 3){
                    // sair
                    printf(" %s%s\n"RESET, fundoSelecaoSair, opcoes[i-1]);
                } else {
                    printf(" %s%s\n"RESET, fundoSelecaoNormal, opcoes[i-1]);
                }
            } else {
                if(i == 3){
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
        char * color = RED;
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

/// @brief função para manipular o menu de deletar chamado fechado 
void chamadosFechados(void){
    esperar_tamanho_minimo(42, 60);
    if(terminalPequenoAlertado){   
        terminalPequenoAlertado = 0;
    }
    clear();
    int totalPaginas = 0;
    int ** divisaoPaginas = NULL;
    int ** divisaoLinhas = NULL;
    carregarDadosDeletarChamado(&totalPaginas, &divisaoPaginas, &divisaoLinhas);
    int paginaAtual = 1;
    int selected = 1;

    printarMenuDeletarChamado(divisaoPaginas, totalPaginas, paginaAtual, selected);
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
                printarMenuDeletarChamado(divisaoPaginas, totalPaginas, paginaAtual, selected);
                terminalPequenoAlertado = 0;
            }
        }else if(k == KC_ESC){
            menuHandler* temp = (menuHandler*)popPilha(estruturasGlobais.pil); // tira o menu de atender chamado
            free(temp); // tira o menu de atender chamado
            break;
        }else if(k == KC_UP){
            if(selected == 2 && paginaAtual == 1){
                updateOption(4, "Deletar todos os chamados fechados", BG_BLUE, "");
                updateOption(5, "Voltar", "", BLUE);
                selected--;
            }else if(selected == 3 && paginaAtual == 1 && ultimaOpcao > 3){
                updateOption(5, "Voltar", BG_BLUE, "");
                updateOption(6, "Sair", "", RED);
                selected--;
            }else if(selected == 4 && paginaAtual == 1 && ultimaOpcao > 3){
                // desmarcar chamado
                updateChamadoFechado(paginaAtual, selected - 3, divisaoLinhas, divisaoPaginas, 0);
                selected--;
                // marcar sair
                updateOption(6, "Sair", BG_RED, "");
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
                        ultimaOpcaoTemp += 3; // ajustar para voltar e sair
                    }
                    selected = ultimaOpcaoTemp;
                    clear();
                    printarMenuDeletarChamado(divisaoPaginas, totalPaginas, paginaAtual, selected);
                }else if(selected > 1){
                    int selectedPrev = selected;
                    if(paginaAtual == 1){
                        selectedPrev -= 3; // ajustar para pular opcoes voltar e sair
                    }
                    updateChamadoFechado(paginaAtual, selectedPrev, divisaoLinhas, divisaoPaginas, 0);
                    selected--;
                    selectedPrev--;
                    updateChamadoFechado(paginaAtual, selectedPrev, divisaoLinhas, divisaoPaginas, 1);
                }
            }
        }else if(k == KC_DOWN){
            if(selected == 1 && paginaAtual == 1){
                // desmarcar voltar
                updateOption(4, "Deletar todos os chamados fechados", "", BLUE);
                // marcar sair
                updateOption(5, "Voltar", BG_BLUE, "");
                selected++;
            }else if(selected == 2 && paginaAtual == 1 ){
                // desmarcar sair
                updateOption(5, "Voltar", "", BLUE);
                selected++;
                updateOption(6, "Sair", BG_RED, "");
            }else if(selected == 3 && paginaAtual == 1 && ultimaOpcao > 3){
                // desmarcar sair
                updateOption(6, "Sair", "", RED);
                selected++;
                updateChamadoFechado(paginaAtual, selected - 3, divisaoLinhas, divisaoPaginas, 1);
            }else{
                if(selected == ultimaOpcao && paginaAtual < totalPaginas){
                    selected = 1;
                    paginaAtual++;
                    clear();
                    printarMenuDeletarChamado(divisaoPaginas, totalPaginas, paginaAtual, selected);
                }else if(selected < ultimaOpcao){
                    int selectedPrev = selected;
                    if(paginaAtual == 1){
                        selectedPrev -= 3; // ajustar para pular opcoes voltar e sair
                    }
                    updateChamadoFechado(paginaAtual, selectedPrev, divisaoLinhas, divisaoPaginas, 0);
                    selected++;
                    selectedPrev++;
                    updateChamadoFechado(paginaAtual, selectedPrev, divisaoLinhas, divisaoPaginas, 1);
                }
            }
        }else if(k == KC_RIGHT){
            if(paginaAtual < totalPaginas){
                paginaAtual++;
                selected = 1;
                clear();
                printarMenuDeletarChamado(divisaoPaginas, totalPaginas, paginaAtual, selected);
            }
        }else if(k == KC_LEFT){
            if(paginaAtual > 1){
                paginaAtual--;
                selected = 1;
                clear();
                printarMenuDeletarChamado(divisaoPaginas, totalPaginas, paginaAtual, selected);
            }
        }else if(k == KC_ENTER){
            if(paginaAtual == 1 && selected == 1){
                // deletar todos chamados fechados
                menuHandler* m = createMenu(20);
                pushPilha(estruturasGlobais.pil, m);
                lastSelected = 1;
                break;
            }else if(paginaAtual == 1 && selected == 2){
                // voltar
                menuHandler* temp = (menuHandler*)popPilha(estruturasGlobais.pil); // tira o menu de atender chamado
                free(temp); // tira o menu de atender chamado
                lastSelected = 2;
                break;
                // apenas por seguranca (isso nao vai ser usado)
                break;
            }else if(selected == 3 && paginaAtual == 1){
                // sair
                menuHandler* m = createMenu(9);
                pushPilha(estruturasGlobais.pil, m);
                lastSelected = 3;
                break;
            }else {
                int countTotal = 0; // qual chamado foi escolhido (numero absoluto na fila)
                for(int i = 0; i < paginaAtual -1; i++){
                    int countPagina = 0;
                    while(divisaoPaginas[i][countPagina] > 0){
                        countTotal++;
                        countPagina++;
                    }
                }
                if(paginaAtual == 1){
                    countTotal += selected - 3; // ajustar para pular opcoes voltar e sair
                } else {
                    countTotal += selected;
                }
                // concluir chamado selecionado
                menuHandler* m = createMenu(19);
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
}