#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "all.h"

/// @brief marcar ou desmarcar um chamado no menu de cancelar chamado 
/// @param paginaAtual pagina atual
/// @param selected chamado selecionado
/// @param divisaoLinhas divisao de linhas
/// @param marcar 1 para marcar, 0 para desmarcar
static void updateChamado(int paginaAtual, int selected, int ** divisaoLinhas, int ** divisaoPaginas, int marcar){
    int linhaInicio = divisaoLinhas[paginaAtual-1][selected - 1];
    if(linhaInicio == 0) linhaInicio = 1; // chamado nao existe
    printf("\033[%d;1H\033[K", linhaInicio); // move para linha e coluna 1 e limpa a linha
    chamado* cham = NULL;
    if(tipoDeMenuVariavel == 1){
        filaPrioridade* f = estruturasGlobais.chamadosAbertosComPrioridade;
        fila * f2 = estruturasGlobais.chamadosAbertosSemPrioridade;
        noFila * no = f2->first;
        int quantChamadosPrioridade = 0;
        for(int i = 0; i < paginaAtual - 1; i++){
            // avancar no noFila
            int countPagina = 0;
            while(divisaoPaginas[i][countPagina] > 0){
                if(quantChamadosPrioridade < f->n){
                    quantChamadosPrioridade++;
                }else{
                    no = no->prox;
                }
                countPagina++;
            }
        }
        for(int i = 1; i < selected; i++){
            if(quantChamadosPrioridade < f->n){
                    quantChamadosPrioridade++;
            }else{
                no = no->prox;
            }
        }
        if(quantChamadosPrioridade < f->n){
            cham = (chamado*)f->elementos[quantChamadosPrioridade];
        }else{
            cham = (chamado*)no->dado;
        }
    }else if(tipoDeMenuVariavel == 2){
        filaDupla* f = estruturasGlobais.chamadosAndamento;
        noDuplo * no = f->inicio;
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
    }else if(tipoDeMenuVariavel == 3){
        filaDupla* fd = estruturasGlobais.chamadosSuspensos;
        noDuplo * no = fd->inicio;
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
    }   
    if(cham == NULL) return;
    char* color = marcar ? BLUE : (tipoDeMenuVariavel == 1 ? GREEN : (tipoDeMenuVariavel == 2 ? YELLOW : RED));
    // printar chamado
    printf("%s+----------------------------------------------------+\n"RESET, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %s%-50s %s|\n"RESET, color, BLUE, (*cham).titulo, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    char bufPrioridade[15] = "Maxima";
    switch(cham->prioridade){
        case 0:
            strcpy(bufPrioridade, "Minima");
            break;
        case 1:
            strcpy(bufPrioridade, "Muito Baixa");
            break;
        case 2:
            strcpy(bufPrioridade, "Baixa");
            break;
        case 3:
            strcpy(bufPrioridade, "Media");
            break;
        case 4:
            strcpy(bufPrioridade, "Alta");
            break;
        case 5:
            strcpy(bufPrioridade, "Urgente");
            break;
    }
    printf("%s| %sPrioridade: %s%-38s %s|\n", color, MAGENTA, WHITE, bufPrioridade, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %sCriador: %s%-41s %s|\n"RESET, color, MAGENTA, WHITE, cham->criador, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    if(tipoDeMenuVariavel == 2){
        printf("%s| %sAtendente: %s%-39s %s|\n"RESET, color, MAGENTA, WHITE, cham->atendente, color);
        printf("\033[%d;1H\033[K", ++linhaInicio);
    }
    printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
    color, MAGENTA, WHITE,
    cham->tempoComplexo.dia, cham->tempoComplexo.mes, cham->tempoComplexo.ano,
    cham->tempoComplexo.horas, cham->tempoComplexo.minutos, cham->tempoComplexo.segundos,
    "", color);
    printf("\033[%d;1H\033[K", ++linhaInicio);
    
    fila * fMat = cham->materiais;
    noFila * noMat = fMat->first;
    for(int i = 0; i < cham->quantMateriais; i++){
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "%s (%d)", ((Item*)noMat->dado)->nome, cham->quantMateriaisPorItem[i]);
        printf("%s| %s%-50s %s|\n"RESET, color, WHITE, buffer, color);
        printf("\033[%d;1H\033[K", ++linhaInicio);
        noMat = noMat->prox;
    }

    printf("%s| %-50s %s|\n"RESET, color, "", color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    char descricao[7][50] = {0};
    int linhas = quebrarDescricao(cham->descricao, descricao);
    // descricao
    for(int i = 0; i < linhas; i++){
        printf("%s| %s%-50s %s|\n"RESET, color, CYAN, descricao[i], color);
        printf("\033[%d;1H\033[K", ++linhaInicio);
    }
    // separador
    printf("%s+----------------------------------------------------+"RESET, color);
    
}

/// @brief carregar os dados do menu de cancelar chamado 
/// @param totalPaginas total de paginas
/// @param divisaoPaginas matriz de divisao de paginas
/// @param divisaoLinhas matriz de divisao de linhas
static void carregarDados(int * totalPaginas, int ***divisaoPaginas, int *** divisaoLinhas){
    *totalPaginas = 1;
    int count = 0;
    int linhasAtual = 8; // primeira pagina tem header;
    chamado * cham;
    
    if(tipoDeMenuVariavel == 1){
        filaPrioridade * f = estruturasGlobais.chamadosAbertosComPrioridade;
        fila * f2 = estruturasGlobais.chamadosAbertosSemPrioridade;
        noFila * no = f2->first;
        int contadorChamadosPrioridade = 0;
        while(no != NULL || contadorChamadosPrioridade < f->n){
            if(contadorChamadosPrioridade < f->n){
                cham = (chamado*)f->elementos[contadorChamadosPrioridade];
            }
            else{
                cham = (chamado*)no->dado;
            }
            // descricao
            char temp[7][50];
            int linhasDesc = quebrarDescricao(cham->descricao, temp);
            linhasAtual += linhasDesc + 7 + cham->quantMateriais; // 7 linhas fixas + linhas da desc
            if(linhasAtual > 40){
                (*totalPaginas)++;
                linhasAtual = linhasDesc + 7 + cham->quantMateriais; // resetar linhasAtual para o novo chamado
            }
            count++;
            if(contadorChamadosPrioridade < f->n){
                contadorChamadosPrioridade++;
            }
            else{
                no = no->prox;
            }
        }
    }else if(tipoDeMenuVariavel == 2){
        filaDupla* f = estruturasGlobais.chamadosAndamento;
        noDuplo * no = f->inicio;
        while(no != NULL){
            cham = (chamado*)no->dado;
            // descricao
            char temp[7][50];
            int linhasDesc = quebrarDescricao(cham->descricao, temp);
            linhasAtual += linhasDesc + 8 + cham->quantMateriais; // 7 linhas fixas + linhas da desc
            if(linhasAtual > 40){
                (*totalPaginas)++;
                linhasAtual = linhasDesc + 8 + cham->quantMateriais; // resetar linhasAtual para o novo chamado
            }
            count++;
            no = no->prox;
        }
        
    }else if(tipoDeMenuVariavel == 3){
        filaDupla* fd = estruturasGlobais.chamadosSuspensos;
        noDuplo * no = fd->inicio;
        while(no != NULL){
            cham = (chamado*)no->dado;
            // descricao
            char temp[7][50];
            int linhasDesc = quebrarDescricao(cham->descricao, temp);
            linhasAtual += linhasDesc + 7 + cham->quantMateriais; // 7 linhas fixas + linhas da desc
            if(linhasAtual > 40){
                (*totalPaginas)++;
                linhasAtual = linhasDesc + 7 + cham->quantMateriais; // resetar linhasAtual para o novo chamado
            }
            count++;
            no = no->prox;
        }
    }
    
    // alocar divisao paginas
    *divisaoPaginas = calloc(*totalPaginas, sizeof(int*));
    for (int i = 0; i < *totalPaginas; i++)
        (*divisaoPaginas)[i] = calloc(10, sizeof(int));

    *divisaoLinhas = calloc(*totalPaginas, sizeof(int*));
    for (int i = 0; i < *totalPaginas; i++)
        (*divisaoLinhas)[i] = calloc(20, sizeof(int));
    
    // preencher divisao paginas e linhas
    if(tipoDeMenuVariavel == 1){
        filaPrioridade * f = estruturasGlobais.chamadosAbertosComPrioridade;
        fila * f2 = estruturasGlobais.chamadosAbertosSemPrioridade;
        noFila * no = f2->first;
        int paginaAtual = 0;
        int chamadoAtualNaPagina = 0;
        int contadorChamadosPrioridade = 0;
        linhasAtual = 8; // primeira pagina tem header
        (*divisaoLinhas)[0][0] = 9; // primeira linha do primeiro chamado
        while(no != NULL || contadorChamadosPrioridade < f->n){
            if(contadorChamadosPrioridade < f->n){
                cham = (chamado*)f->elementos[contadorChamadosPrioridade];
            }
            else{
                cham = (chamado*)no->dado;
            }
            // descricao
            char temp[7][50];
            int linhasDesc = quebrarDescricao(cham->descricao, temp);
            linhasAtual += linhasDesc + 7 + cham->quantMateriais; // 7 linhas fixas + linhas da desc
            if(linhasAtual > 40){
                // nova pagina
                paginaAtual++;
                linhasAtual = linhasDesc + 7 + cham->quantMateriais; // resetar linhasAtual para o novo chamado
                chamadoAtualNaPagina = 0;
            }
            
            // preencher divisao paginas e linhas
            (*divisaoPaginas)[paginaAtual][chamadoAtualNaPagina] = 1;
            (*divisaoLinhas)[paginaAtual][chamadoAtualNaPagina + 1] = linhasAtual + 1; // marcar linha inicial do chamado
            chamadoAtualNaPagina++;
            if(contadorChamadosPrioridade < f->n){
                contadorChamadosPrioridade++;
            }
            else{
                no = no->prox;
            }
        }
    
    }else if(tipoDeMenuVariavel == 2){
        filaDupla* f = estruturasGlobais.chamadosAndamento;
        noDuplo * no = f->inicio;
        int paginaAtual = 0;
        int chamadoAtualNaPagina = 0;
        linhasAtual = 8; // primeira pagina tem header
        (*divisaoLinhas)[0][0] = 9; // primeira linha do primeiro chamado
        while(no != NULL){
            cham = (chamado*)no->dado;
            // descricao
            char temp[7][50];
            int linhasDesc = quebrarDescricao(cham->descricao, temp);
            linhasAtual += linhasDesc + 8 + cham->quantMateriais; // 7 linhas fixas + linhas da desc
            if(linhasAtual > 40){
                // nova pagina
                paginaAtual++;
                linhasAtual = linhasDesc + 8 + cham->quantMateriais; // resetar linhasAtual para o novo chamado
                chamadoAtualNaPagina = 0;
            }
            
            // preencher divisao paginas e linhas
            (*divisaoPaginas)[paginaAtual][chamadoAtualNaPagina] = (count > 0) ? 1 : 0; // marcar que tem chamado ou que nao tem
            (*divisaoLinhas)[paginaAtual][chamadoAtualNaPagina + 1] = linhasAtual + 1; // marcar linha inicial do chamado
            chamadoAtualNaPagina++;
            no = no->prox;
        }
    
    }else if(tipoDeMenuVariavel == 3){
        filaDupla* fd = estruturasGlobais.chamadosSuspensos;
        noDuplo * no = fd->inicio;
        int paginaAtual = 0;
        int chamadoAtualNaPagina = 0;
        linhasAtual = 8; // primeira pagina tem header
        (*divisaoLinhas)[0][0] = 9; // primeira linha do primeiro chamado
        while(no != NULL){
            cham = (chamado*)no->dado;
            // descricao
            char temp[7][50];
            int linhasDesc = quebrarDescricao(cham->descricao, temp);
            linhasAtual += linhasDesc + 7 + cham->quantMateriais; // 7 linhas fixas + linhas da desc
            if(linhasAtual > 40){
                // nova pagina
                paginaAtual++;
                linhasAtual = linhasDesc + 7 + cham->quantMateriais; // resetar linhasAtual para o novo chamado
                chamadoAtualNaPagina = 0;
            }
            
            // preencher divisao paginas e linhas
            (*divisaoPaginas)[paginaAtual][chamadoAtualNaPagina] = (count > 0) ? 1 : 0; // marcar que tem chamado ou que nao tem
            (*divisaoLinhas)[paginaAtual][chamadoAtualNaPagina + 1] = linhasAtual + 1; // marcar linha inicial do chamado
            chamadoAtualNaPagina++;
            no = no->prox;
        }
    }
    
}

/// @brief printar o menu de cancelar chamado 
/// @param divisaoPaginas matriz de divisao de paginas
/// @param totalPaginas total de paginas
/// @param paginaAtual pagina atual
/// @param selected chamado ou opcao selecionada
static void printarCancelarChamado(int ** divisaoPaginas, int totalPaginas, int paginaAtual, int selected){
    int count = 0;
    if(paginaAtual == 1){
        // primeira pagina tem 13 linhas a menos (header)
        char* titulo = "Cancelar Chamado";
        repetirChar(15, '-', BLUE);
        printf(BLUE"%s"RESET, titulo);
        repetirChar(15, '-', BLUE);
        printf("\n");
        printf(GREEN" %s"RESET, "Selecione um chamado para cancelar");
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
    filaPrioridade * f = estruturasGlobais.chamadosAbertosComPrioridade;
    fila * f2 = estruturasGlobais.chamadosAbertosSemPrioridade;
    noFila * no = f2->first;
    filaDupla * fa = estruturasGlobais.chamadosAndamento;
    noDuplo * noa = fa->inicio;
    filaDupla * fs = estruturasGlobais.chamadosSuspensos;
    noDuplo * nos = fs->inicio;
    int contadorPrioridade = 0;
    if(tipoDeMenuVariavel == 1){
        for(int i = 0; i < paginaAtual -1; i++){
            int countPagina = 0;
            while(divisaoPaginas[i][countPagina] > 0){
                if(contadorPrioridade < f->n){
                    contadorPrioridade++;
                }else{
                    no = no->prox;
                }
                countPagina++;
            }
        }
    }else if(tipoDeMenuVariavel == 2){
        for(int i = 0; i < paginaAtual -1; i++){
            int countPagina = 0;
            while(divisaoPaginas[i][countPagina] > 0){
                noa = noa->prox;
                countPagina++;
            }
        }
    }else if(tipoDeMenuVariavel == 3){
        for(int i = 0; i < paginaAtual -1; i++){
            int countPagina = 0;
            while(divisaoPaginas[i][countPagina] > 0){
                nos = nos->prox;
                countPagina++;
            }
        }
    }
    chamado* cham = NULL;
    char * color = NULL;
    
    while(divisaoPaginas[paginaAtual-1][count] > 0){
        if(tipoDeMenuVariavel == 1){
        if(contadorPrioridade < f->n){
            cham = (chamado*)f->elementos[contadorPrioridade];
        }
        else{
            cham = (chamado*)no->dado;
        }
        color = GREEN;
        }else if(tipoDeMenuVariavel == 2){
            cham = (chamado*)noa->dado;
            color = YELLOW;
        }else if(tipoDeMenuVariavel == 3){
            cham = (chamado*)nos->dado;
            color = RED;
        }
        
        if((selected == count + 3 && paginaAtual == 1) || (selected == count +1 && paginaAtual > 1)){
            // chamado selecionado
            color = BLUE;
        }
        printf("%s+----------------------------------------------------+\n"RESET, color);
        printf("%s| %s%-50s %s|\n"RESET, color, BLUE, cham->titulo, color);
        
        // prioridade
        char bufPrioridade[15] = "Maxima";
        switch(cham->prioridade){
            case 0:
                strcpy(bufPrioridade, "Minima");
                break;
            case 1:
                strcpy(bufPrioridade, "Muito Baixa");
                break;
            case 2:
                strcpy(bufPrioridade, "Baixa");
                break;
            case 3:
                strcpy(bufPrioridade, "Media");
                break;
            case 4:
                strcpy(bufPrioridade, "Alta");
                break;
            case 5:
                strcpy(bufPrioridade, "Urgente");
                break;
        }
        printf("%s| %sPrioridade: %s%-38s %s|\n", color, MAGENTA, WHITE, bufPrioridade, color);
        
        // criador
        printf("%s| %sCriador: %s%-41s %s|\n"RESET, color, MAGENTA, WHITE, cham->criador, color);
        
        // atendente (caso for em andamento)
        if(tipoDeMenuVariavel == 2){
            printf("%s| %sAtendente: %s%-39s %s|\n"RESET, color, MAGENTA, WHITE, cham->atendente, color);
        }
        // data e hora
        printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
        color, MAGENTA, WHITE,
        cham->tempoComplexo.dia, cham->tempoComplexo.mes, cham->tempoComplexo.ano,
        cham->tempoComplexo.horas, cham->tempoComplexo.minutos, cham->tempoComplexo.segundos,
        "", color);

        // materiais
        fila * fMat = cham->materiais;
        noFila * noMat = fMat->first;
        for(int i = 0; i < cham->quantMateriais; i++){
            char buffer[50];
            snprintf(buffer, sizeof(buffer), "%s (%d)", ((Item*)noMat->dado)->nome, cham->quantMateriaisPorItem[i]);
            printf("%s| %s%-50s %s|\n"RESET, color, WHITE, buffer, color);
            noMat = noMat->prox;
        }
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
        if(tipoDeMenuVariavel == 1){
            if(contadorPrioridade < f->n){
                contadorPrioridade++;
            }
            else{
                no = no->prox;
            }
            color = GREEN;
        }else if(tipoDeMenuVariavel == 2){
            noa = noa->prox;
        }else if(tipoDeMenuVariavel == 3){
            nos = nos->prox;
        }
    }
    
    printf("\033[42;1H"); // ir pra linha 42 coluna 1
    printf(BLUE" Pagina %d/%d"RESET, paginaAtual, totalPaginas);
}

/// @brief função para manipular o menu de cancelar chamados
void menuCancelarChamados(void){
    esperar_tamanho_minimo(42, 60);
    if(terminalPequenoAlertado){   
        terminalPequenoAlertado = 0;
    }
    clear();
    int totalPaginas = 0;
    int ** divisaoPaginas = NULL;
    int ** divisaoLinhas = NULL;
    carregarDados(&totalPaginas, &divisaoPaginas, &divisaoLinhas);
    int paginaAtual = 1;
    int selected = 1;

    printarCancelarChamado(divisaoPaginas, totalPaginas, paginaAtual, selected);
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
                printarCancelarChamado(divisaoPaginas, totalPaginas, paginaAtual, selected);
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
                updateChamado(paginaAtual, selected - 2, divisaoLinhas, divisaoPaginas, 0);
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
                    printarCancelarChamado(divisaoPaginas, totalPaginas, paginaAtual, selected);
                }else if(selected > 1){
                    int selectedPrev = selected;
                    if(paginaAtual == 1){
                        selectedPrev -= 2; // ajustar para pular opcoes voltar e sair
                    }
                    updateChamado(paginaAtual, selectedPrev, divisaoLinhas, divisaoPaginas, 0);
                    selected--;
                    selectedPrev--;
                    updateChamado(paginaAtual, selectedPrev, divisaoLinhas, divisaoPaginas, 1);
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
                updateChamado(paginaAtual, selected - 2, divisaoLinhas, divisaoPaginas, 1);
            }else{
                if(selected == ultimaOpcao && paginaAtual < totalPaginas){
                    selected = 1;
                    paginaAtual++;
                    clear();
                    printarCancelarChamado(divisaoPaginas, totalPaginas, paginaAtual, selected);
                }else if(selected < ultimaOpcao){
                    int selectedPrev = selected;
                    if(paginaAtual == 1){
                        selectedPrev -= 2; // ajustar para pular opcoes voltar e sair
                    }
                    updateChamado(paginaAtual, selectedPrev, divisaoLinhas, divisaoPaginas, 0);
                    selected++;
                    selectedPrev++;
                    updateChamado(paginaAtual, selectedPrev, divisaoLinhas, divisaoPaginas, 1);
                }
            }
        }else if(k == KC_RIGHT){
            if(paginaAtual < totalPaginas){
                paginaAtual++;
                selected = 1;
                clear();
                printarCancelarChamado(divisaoPaginas, totalPaginas, paginaAtual, selected);
            }
        }else if(k == KC_LEFT){
            if(paginaAtual > 1){
                paginaAtual--;
                selected = 1;
                clear();
                printarCancelarChamado(divisaoPaginas, totalPaginas, paginaAtual, selected);
            }
        }else if(k == KC_ENTER){
            if(paginaAtual == 1 && selected == 1){
                // voltar
                menuHandler* temp = (menuHandler*)popPilha(estruturasGlobais.pil); // tira o menu de atender chamado
                free(temp); // tira o menu de atender chamado
                
                break;
            }else if(paginaAtual == 1 && selected == 2){
                // sair
                menuHandler* m = createMenu(9);
                pushPilha(estruturasGlobais.pil, m);
            
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
                menuHandler* m = NULL;
                if(tipoDeMenuVariavel == 1){
                    m = createMenu(29);
                }else if(tipoDeMenuVariavel == 2){
                    m = createMenu(30);
                }else{
                    m = createMenu(31);
                }

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