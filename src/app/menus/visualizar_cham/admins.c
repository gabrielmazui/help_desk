#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define TAM_LINHA 52

#include "all.h"

/// @brief  Cria um vetor ordenado dos chamados na fila de prioridade
/// @param fp Ponteiro para a fila de prioridade
/// @return Um vetor de chamados ordenados
static chamado** vetorOrdenadoFilaPrioridade(filaPrioridade* fp) {
    filaPrioridade* fpCopia = filaPrioridadeCopiar(fp);
    // para conseguir pegar o topo sempre e colocar em um vetor
    chamado** vetor = NULL;
    if(fp->n > 0){
        vetor = (chamado**)malloc(fp->n * sizeof(chamado*));
    }
    
    // colocar em ordem no vetor
    for(int i = 0; i < fp->n; i++){
        vetor[i] = (chamado*)filaPrioridadeRemover(fpCopia);
    }

    filaPrioridadeLiberar(&fpCopia);
    return vetor;
}

/// @brief Carrega os buffers de chamados para exibição
/// @param buf1 buffer de chamados abertos
/// @param buf2 buffer de chamados em andamento
/// @param buf3 buffer de chamados suspensos
/// @param linhasAberto número de linhas no buffer de chamados abertos
/// @param linhasAndamento número de linhas no buffer de chamados em andamento
/// @param linhasFechado número de linhas no buffer de chamados suspensos
/// @param divisaoLinhas matriz de divisao de linhas por pagina de cada buffer
/// @param totalPaginas número total de páginas necessárias para exibição
static void carregarBufferChamados(char ***buf1, char ***buf2, char ***buf3, int* linhasAberto, int* linhasAndamento, int* linhasFechado, int*** divisaoLinhas, int* totalPaginas) {
    
    fila *f = estruturasGlobais.chamadosAbertosSemPrioridade;
    filaPrioridade *fp = estruturasGlobais.chamadosAbertosComPrioridade;
    filaDupla *fd = estruturasGlobais.chamadosSuspensos;
    filaDupla *fAndamento = estruturasGlobais.chamadosAndamento;

    noFila *atualFilaAberto = f ? f->first : NULL;
    noDuplo *atualFilaFechado = fd ? fd->inicio : NULL;
    noDuplo *atualFilaAndamento = fAndamento ? fAndamento->inicio : NULL;
    chamado** vetPrioridadeTemp = vetorOrdenadoFilaPrioridade(fp);
    
    int fQuant = f ? f->n : 0;
    int fdQuant = fd ? fd->n : 0;
    int fAndamentoQuant = fAndamento ? fAndamento->n : 0;
    int fpQuant = fp ? fp->n : 0;

    int fUsed = 0, fdUsed = 0, fAndamentoUsed = 0, fpUsed = 0;

    // =============================================
    // CONTAGEM DE LINHAS POR CHAMADO
    // =============================================

    // Buffers auxiliares para contagem de linhas por chamado
    int *linhasPorChamadoAberto = malloc((fQuant + fpQuant) * sizeof(int));
    int *linhasPorChamadoAndamento = malloc(fAndamentoQuant * sizeof(int));
    int *linhasPorChamadoFechado = malloc(fdQuant * sizeof(int));

    // contador geral
    int totalLinhasAberto = 0, totalLinhasAndamento = 0, totalLinhasFechado = 0;

    // ----------- Abertos (fila normal + prioridade) -----------
    {
        int index = 0;
        // fila de prioridade (vetor)
        if (fp && fp->elementos) {
            for (int i = 0; i < fp->n; i++) {
                char tempDestino[7][50];
                int linhasDescricao = quebrarDescricao((*vetPrioridadeTemp[i]).descricao, tempDestino);
                int linhasTotais = linhasDescricao + 6 + ((chamado*)(fp->elementos[i]))->quantMateriais;

                linhasPorChamadoAberto[index++] = linhasTotais;
                totalLinhasAberto += linhasTotais;
            }
        }

        // fila normal
        noFila *atual = f ? f->first : NULL;
        while (atual) {
            char tempDestino[7][50];
            int linhasDescricao = quebrarDescricao((*((chamado*)(atual->dado))).descricao, tempDestino);
            int linhasTotais = linhasDescricao + 6 + ((chamado*)(atual->dado))->quantMateriais;

            linhasPorChamadoAberto[index++] = linhasTotais;
            totalLinhasAberto += linhasTotais;

            atual = atual->prox;
        }

    }

    // ----------- Andamento -----------
    {
        int index = 0;
        noDuplo *atual = fAndamento ? fAndamento->inicio : NULL;
        while (atual) {
            char tempDestino[7][50];
            int linhasDescricao = quebrarDescricao((*((chamado*)(atual->dado))).descricao, tempDestino);
            int linhasTotais = linhasDescricao + 7 + ((chamado*)(atual->dado))->quantMateriais;
            // 6 + 1 (atendente)
            linhasPorChamadoAndamento[index++] = linhasTotais;
            totalLinhasAndamento += linhasTotais;

            atual = atual->prox;
        }
    }

    // ----------- Fechados -----------
    {
        int index = 0;
        noDuplo *atual = fd ? fd->inicio : NULL;
        while (atual) {
            char tempDestino[7][50];
            int linhasDescricao = quebrarDescricao((*((chamado*)(atual->dado))).descricao, tempDestino);
            int linhasTotais = linhasDescricao + 6 + ((chamado*)(atual->dado))->quantMateriais;
            // 6 + 1 (atendido por)
            linhasPorChamadoFechado[index++] = linhasTotais;
            totalLinhasFechado += linhasTotais;

            atual = atual->prox;
        }
    }

    // define as quantidades totais de linhas com base real
    *linhasAberto = totalLinhasAberto;
    *linhasAndamento = totalLinhasAndamento;
    *linhasFechado = totalLinhasFechado;

    *buf1 = NULL;
    *buf2 = NULL;
    *buf3 = NULL;
    // alocar matrizAberto se tiver linhas
    if (*linhasAberto > 0) {
        *buf1 = calloc(*linhasAberto, sizeof(char*));
        if (!(*buf1)){ 
            perror("calloc matrizAberto"); exit(1); 
        }
        for (int i = 0; i < *linhasAberto; i++) {
            (*buf1)[i] = calloc(52, sizeof(char)); // 51 + '\0'
            if (!(*buf1)[i]) { 
                perror("calloc matrizAberto[i]"); exit(1); 
            }
        }
    }

    // alocar matrizAndamento se tiver linhas
    if (*linhasAndamento > 0) {
        *buf2 = calloc(*linhasAndamento, sizeof(char*));
        if (!*buf2) {   
            perror("calloc matrizAndamento"); exit(1); 
        }
        for (int i = 0; i < *linhasAndamento; i++) {
            (*buf2)[i] = calloc(52, sizeof(char));
            if (!(*buf2)[i]) { 
                perror("calloc matrizAndamento[i]"); exit(1); 
            }
        }
    }

    // alocar matrizFechado se tiver linhas
    if (*linhasFechado > 0) {
        *buf3 = calloc(*linhasFechado, sizeof(char*));
        if (!*buf3) { perror("calloc matrizFechado"); exit(1); }
        for (int i = 0; i < *linhasFechado; i++) {
            (*buf3)[i] = calloc(52, sizeof(char));
            if (!(*buf3)[i]) { perror("calloc matrizFechado[i]"); exit(1); }
        }
    }
    
    
    char **matrizAberto = *buf1;
    char **matrizAndamento = *buf2;
    char **matrizFechado = *buf3;

    int linhaAtualAberto = 0;
    int linhaAtualAndamento = 0;
    int linhaAtualFechado = 0;
    // 0 - linha vazia
    // 1 - titulo
    // 2 - prioridade
    // 3 - criador
    // 4 - atendente / atendido por (nao tem nos abertos)
    // 5 - data e hora
    // 6 - materiais
    // 7 - descricao
    // 8 - separador
    
    while(fUsed < fQuant || fdUsed < fdQuant || fAndamentoUsed < fAndamentoQuant || fpUsed < fpQuant) {
        // ========================================================
        // (TÍTULO)
        // ========================================================
        if (*linhasAberto > 0 && linhaAtualAberto < *linhasAberto) {
            matrizAberto[linhaAtualAberto][0] = '\0';
        }
        if (*linhasAndamento > 0 && linhaAtualAndamento < *linhasAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }
        if (*linhasFechado > 0 && linhaAtualFechado < *linhasFechado) {
            matrizFechado[linhaAtualFechado][0] = '\0';
        }

        if (fpUsed < fpQuant) {
            matrizAberto[linhaAtualAberto][0] = '1'; // marca como titulo
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s", (*vetPrioridadeTemp[fpUsed]).titulo);
            linhaAtualAberto++;
        } else if (fUsed < fQuant && atualFilaAberto) {
            matrizAberto[linhaAtualAberto][0] = '1'; // marca como titulo
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s", (*((chamado*)atualFilaAberto->dado)).titulo);
            linhaAtualAberto++;
        }
        if (fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '1'; // marca como titulo
            snprintf(matrizAndamento[linhaAtualAndamento] + 1, TAM_LINHA - 1, "%s", (*((chamado*)atualFilaAndamento->dado)).titulo);
            linhaAtualAndamento++;
        }
        
        fflush(stdout);
        if (fdUsed < fdQuant && atualFilaFechado) {
            matrizFechado[linhaAtualFechado][0] = '1'; // marca como titulo
            snprintf(matrizFechado[linhaAtualFechado] + 1, TAM_LINHA - 1, "%s", (*((chamado*)atualFilaFechado->dado)).titulo);
            linhaAtualFechado++;
        }
        
        // ========================================================
        // Linha 2: PRIORIDADE
        // ========================================================
        if (*linhasAberto > 0 && linhaAtualAberto < *linhasAberto) {
            matrizAberto[linhaAtualAberto][0] = '\0';
        }
        if (*linhasAndamento > 0 && linhaAtualAndamento < *linhasAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }
        if (*linhasFechado > 0 && linhaAtualFechado < *linhasFechado) {
            matrizFechado[linhaAtualFechado][0] = '\0';
        }

        if (fpUsed < fpQuant) {
            matrizAberto[linhaAtualAberto][0] = '2'; // marca como prioridade
            char bufferPrioridade[10] = "Maxima";
            switch((*vetPrioridadeTemp[fpUsed]).prioridade) {
                case 0:
                    strcpy(bufferPrioridade, "Minima");
                    break;
                case 1:
                    strcpy(bufferPrioridade, "Baixa");
                    break;
                case 2:
                    strcpy(bufferPrioridade, "Media");
                    break;
                case 3:
                    strcpy(bufferPrioridade, "Alta");
                    break;
                case 4:
                    strcpy(bufferPrioridade, "Urgente");
                    break;
                case 5:
                    strcpy(bufferPrioridade, "Maxima");
                    break;
            }
            
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s", bufferPrioridade);
            linhaAtualAberto++;
        } else if (fUsed < fQuant && atualFilaAberto) {
            char bufferPrioridade[10] = "Maxima";
            switch(((*((chamado*)atualFilaAberto->dado)).prioridade)) {
                case 0:
                    strcpy(bufferPrioridade, "Minima");
                    break;
                case 1:
                    strcpy(bufferPrioridade, "Baixa");
                    break;
                case 2:
                    strcpy(bufferPrioridade, "Media");
                    break;
                case 3:
                    strcpy(bufferPrioridade, "Alta");
                    break;
                case 4:
                    strcpy(bufferPrioridade, "Urgente");
                    break;
                case 5:
                    strcpy(bufferPrioridade, "Maxima");
                    break;
            }
            matrizAberto[linhaAtualAberto][0] = '2'; // marca como prioridade
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s", bufferPrioridade);
            linhaAtualAberto++;
        }

        if (fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            char bufferPrioridade[10] = "Maxima";
            switch(((*((chamado*)atualFilaAndamento->dado)).prioridade)) {
                case 0:
                    strcpy(bufferPrioridade, "Minima");
                    break;
                case 1:
                    strcpy(bufferPrioridade, "Baixa");
                    break;
                case 2:
                    strcpy(bufferPrioridade, "Media");
                    break;
                case 3:
                    strcpy(bufferPrioridade, "Alta");
                    break;
                case 4:
                    strcpy(bufferPrioridade, "Urgente");
                    break;
                case 5:
                    strcpy(bufferPrioridade, "Maxima");
                    break;
            }
            matrizAndamento[linhaAtualAndamento][0] = '2'; // marca como prioridade
            snprintf(matrizAndamento[linhaAtualAndamento] + 1, TAM_LINHA - 1, "%s", bufferPrioridade);
            linhaAtualAndamento++;
        }

        if (fdUsed < fdQuant && atualFilaFechado) {
            char bufferPrioridade[10] = "Maxima";
            switch(((*((chamado*)atualFilaFechado->dado)).prioridade)) {
                case 0:
                    strcpy(bufferPrioridade, "Minima");
                    break;
                case 1:
                    strcpy(bufferPrioridade, "Baixa");
                    break;
                case 2:
                    strcpy(bufferPrioridade, "Media");
                    break;
                case 3:
                    strcpy(bufferPrioridade, "Alta");
                    break;
                case 4:
                    strcpy(bufferPrioridade, "Urgente");
                    break;
                case 5:
                    strcpy(bufferPrioridade, "Maxima");
                    break;
            }
            matrizFechado[linhaAtualFechado][0] = '2'; // marca como prioridade
            snprintf(matrizFechado[linhaAtualFechado] + 1, TAM_LINHA - 1, "%s", bufferPrioridade);
            linhaAtualFechado++;
        }

        // ========================================================
        // Linha 3: CRIADOR
        // ========================================================
        if (*linhasAberto > 0 && linhaAtualAberto < *linhasAberto) {
            matrizAberto[linhaAtualAberto][0] = '\0';
        }
        if (*linhasAndamento > 0 && linhaAtualAndamento < *linhasAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }
        if (*linhasFechado > 0 && linhaAtualFechado < *linhasFechado) {
            matrizFechado[linhaAtualFechado][0] = '\0';
        }

        if (fpUsed < fpQuant) {
            matrizAberto[linhaAtualAberto][0] = '3'; // marca como criador
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s", (*vetPrioridadeTemp[fpUsed]).criador);
            linhaAtualAberto++;
        } else if (fUsed < fQuant && atualFilaAberto) {
            matrizAberto[linhaAtualAberto][0] = '3'; // marca como criador
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s", (*((chamado*)atualFilaAberto->dado)).criador);
            linhaAtualAberto++;
        }
        if (fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '3'; // marca como criador
            snprintf(matrizAndamento[linhaAtualAndamento] + 1, TAM_LINHA - 1, "%s", (*((chamado*)atualFilaAndamento->dado)).criador);
            linhaAtualAndamento++;
        }

        if (fdUsed < fdQuant && atualFilaFechado) {
            matrizFechado[linhaAtualFechado][0] = '3'; // marca como criador
            snprintf(matrizFechado[linhaAtualFechado] + 1, TAM_LINHA - 1, "%s", (*((chamado*)atualFilaFechado->dado)).criador);
            linhaAtualFechado++;
        }
        
        // ========================================================
        // Linha 4: ATENDENTE / ATENDIDO POR
        // ========================================================
        // APENAS PARA ANDAMENTO

        if (*linhasAndamento > 0 && linhaAtualAndamento < *linhasAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }

        if (fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '4'; // marca como atendente
            snprintf(matrizAndamento[linhaAtualAndamento] + 1, TAM_LINHA - 1, "%s", (*((chamado*)atualFilaAndamento->dado)).atendente);
            linhaAtualAndamento++;
        }

        // ========================================================
        // Linha 5: DATA E HORA
        // ========================================================
        if (*linhasAberto > 0 && linhaAtualAberto < *linhasAberto) {
            matrizAberto[linhaAtualAberto][0] = '\0';
        }
        if (*linhasAndamento > 0 && linhaAtualAndamento < *linhasAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }
        if (*linhasFechado > 0 && linhaAtualFechado < *linhasFechado) {
            matrizFechado[linhaAtualFechado][0] = '\0';
        }

        if (fpUsed < fpQuant){
            matrizAberto[linhaAtualAberto][0] = '5'; // marca como data e hora
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%02d/%02d/%04d %02d:%02d:%02d",
                    (*vetPrioridadeTemp[fpUsed]).tempoComplexo.dia,
                    (*vetPrioridadeTemp[fpUsed]).tempoComplexo.mes,
                    (*vetPrioridadeTemp[fpUsed]).tempoComplexo.ano,
                    (*vetPrioridadeTemp[fpUsed]).tempoComplexo.horas,
                    (*vetPrioridadeTemp[fpUsed]).tempoComplexo.minutos,
                    (*vetPrioridadeTemp[fpUsed]).tempoComplexo.segundos);
            linhaAtualAberto++;
        }
        else if (fUsed < fQuant && atualFilaAberto) {
            matrizAberto[linhaAtualAberto][0] = '5'; // marca como data e hora
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%02d/%02d/%04d %02d:%02d:%02d",
                    (*((chamado*)atualFilaAberto->dado)).tempoComplexo.dia,
                    (*((chamado*)atualFilaAberto->dado)).tempoComplexo.mes,
                    (*((chamado*)atualFilaAberto->dado)).tempoComplexo.ano,
                    (*((chamado*)atualFilaAberto->dado)).tempoComplexo.horas,
                    (*((chamado*)atualFilaAberto->dado)).tempoComplexo.minutos,
                    (*((chamado*)atualFilaAberto->dado)).tempoComplexo.segundos);
            linhaAtualAberto++;
        }
        if (fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '5'; // marca como data e hora
            snprintf(matrizAndamento[linhaAtualAndamento] + 1, TAM_LINHA - 1, "%02d/%02d/%04d %02d:%02d:%02d",
                    (*((chamado*)atualFilaAndamento->dado)).tempoComplexo.dia,
                    (*((chamado*)atualFilaAndamento->dado)).tempoComplexo.mes,
                    (*((chamado*)atualFilaAndamento->dado)).tempoComplexo.ano,
                    (*((chamado*)atualFilaAndamento->dado)).tempoComplexo.horas,
                    (*((chamado*)atualFilaAndamento->dado)).tempoComplexo.minutos,
                    (*((chamado*)atualFilaAndamento->dado)).tempoComplexo.segundos);
            linhaAtualAndamento++;
        }
        
        if (fdUsed < fdQuant && atualFilaFechado) {
            matrizFechado[linhaAtualFechado][0] = '5'; // marca como data e hora
            snprintf(matrizFechado[linhaAtualFechado] + 1, TAM_LINHA - 1, "%02d/%02d/%04d %02d:%02d:%02d",
                    (*((chamado*)atualFilaFechado->dado)).tempoComplexo.dia,
                    (*((chamado*)atualFilaFechado->dado)).tempoComplexo.mes,
                    (*((chamado*)atualFilaFechado->dado)).tempoComplexo.ano,
                    (*((chamado*)atualFilaFechado->dado)).tempoComplexo.horas,
                    (*((chamado*)atualFilaFechado->dado)).tempoComplexo.minutos,
                    (*((chamado*)atualFilaFechado->dado)).tempoComplexo.segundos);
            linhaAtualFechado++;
        }
        
        // ========================================================
        // Linha 6: MATERIAIS
        // ========================================================
        if (*linhasAberto > 0 && linhaAtualAberto < *linhasAberto) {
            matrizAberto[linhaAtualAberto][0] = '\0';
        }
        if (*linhasAndamento > 0 && linhaAtualAndamento < *linhasAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }
        if (*linhasFechado > 0 && linhaAtualFechado < *linhasFechado) {
            matrizFechado[linhaAtualFechado][0] = '\0';
        }
        if (fpUsed < fpQuant) {
            int quantMateriais = (*vetPrioridadeTemp[fpUsed]).quantMateriais;
            noFila* temp = vetPrioridadeTemp[fpUsed]->materiais->first;
            for (int i = 0; i < quantMateriais; i++) {
                matrizAberto[linhaAtualAberto][0] = '6'; // marca como material
                snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s (%d)", (((Item*)(temp->dado))->nome), (*vetPrioridadeTemp[fpUsed]).quantMateriaisPorItem[i]);
                linhaAtualAberto++;
                temp = temp->prox;
            }
            
        } else if (fUsed < fQuant && atualFilaAberto) {
            int quantMateriais = (*((chamado*)atualFilaAberto->dado)).quantMateriais;
            noFila* temp = ((chamado*)atualFilaAberto->dado)->materiais->first;
            for (int i = 0; i < quantMateriais; i++) {
                matrizAberto[linhaAtualAberto][0] = '6'; // marca como material
                snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s (%d)",(((Item*)(temp->dado))->nome), (*((chamado*)atualFilaAberto->dado)).quantMateriaisPorItem[i]);
                linhaAtualAberto++;
                temp = temp->prox;
            }
        }
        if(fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            int quantMateriais = (*((chamado*)atualFilaAndamento->dado)).quantMateriais;
            noFila* temp = ((chamado*)atualFilaAndamento->dado)->materiais->first;
            for (int i = 0; i < quantMateriais; i++) {
                matrizAndamento[linhaAtualAndamento][0] = '6'; // marca como material
                snprintf(matrizAndamento[linhaAtualAndamento] + 1, TAM_LINHA - 1, "%s (%d)",(((Item*)(temp->dado))->nome), (*((chamado*)atualFilaAndamento->dado)).quantMateriaisPorItem[i]);
                linhaAtualAndamento++;
                temp = temp->prox;
            }
        }
        if(fdUsed < fdQuant && atualFilaFechado){
            int quantMateriais = (*((chamado*)atualFilaFechado->dado)).quantMateriais;
            noFila* temp = ((chamado*)atualFilaFechado->dado)->materiais->first;
            for (int i = 0; i < quantMateriais; i++) {
                matrizFechado[linhaAtualFechado][0] = '6'; // marca como material
                snprintf(matrizFechado[linhaAtualFechado] + 1, TAM_LINHA - 1, "%s (%d)",(((Item*)(temp->dado))->nome), (*((chamado*)atualFilaFechado->dado)).quantMateriaisPorItem[i]);
                linhaAtualFechado++;
                temp = temp->prox;
            }
        }

        // =============== LINHA VAZIA ==========================================
        if (*linhasAberto > 0 && linhaAtualAberto < *linhasAberto) {
            matrizAberto[linhaAtualAberto][0] = '\0';
        }
        if (*linhasAndamento > 0 && linhaAtualAndamento < *linhasAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }
        if (*linhasFechado > 0 && linhaAtualFechado < *linhasFechado) {
            matrizFechado[linhaAtualFechado][0] = '\0';
        }

        if(fpUsed < fpQuant || (fUsed < fQuant && atualFilaAberto)) {
            matrizAberto[linhaAtualAberto][0] = '0'; // marca como linha vazia
            linhaAtualAberto++;
        }
        if(fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '0'; // marca como linha vazia
            linhaAtualAndamento++;
        }
        if(fdUsed < fdQuant && atualFilaFechado) {
            matrizFechado[linhaAtualFechado][0] = '0'; // marca como linha vazia
            linhaAtualFechado++;
        }
        
        // ========================================================
        // Linha 7+: DESCRIÇÃO (7 linhas max)
        // ========================================================

        if (*linhasAberto > 0 && linhaAtualAberto < *linhasAberto) {
            matrizAberto[linhaAtualAberto][0] = '\0';
        }
        if (*linhasAndamento > 0 && linhaAtualAndamento < *linhasAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }
        if (*linhasFechado > 0 && linhaAtualFechado < *linhasFechado) {
            matrizFechado[linhaAtualFechado][0] = '\0';
        }

        char desc1[7][50] = {0};
        char desc2[7][50] = {0};
        char desc3[7][50] = {0};

        // fila de prioridade
        if (fpUsed < fpQuant) {
            // formatar string descricao (para nao ficar palavras cortadas na hora de printar)
            quebrarDescricao((*vetPrioridadeTemp[fpUsed]).descricao, desc1);
            // guardar a descricao na matriz
            int linha = 0;
            while(desc1[linha][0] != '\0' && linha < 7) {
                matrizAberto[linhaAtualAberto + linha][0] = '7'; // marca como descricao
                snprintf(matrizAberto[linhaAtualAberto + linha] + 1, TAM_LINHA - 1, "%s", desc1[linha]);
                linha++;
            }
            matrizAberto[linhaAtualAberto + linha][0] = '8'; // marca como separador
            linhaAtualAberto += linha + 1;

        // fila normal de aberto
        } else if (fUsed < fQuant && atualFilaAberto) {
            // formatar string descricao (para nao ficar palavras cortadas na hora de printar)
            quebrarDescricao((*((chamado*)atualFilaAberto->dado)).descricao, desc1);
            // guardar a descricao na matriz
            int linha = 0;
            while(desc1[linha][0] != '\0' && linha < 7) {
                matrizAberto[linhaAtualAberto + linha][0] = '7'; // marca como descricao
                snprintf(matrizAberto[linhaAtualAberto + linha] + 1, TAM_LINHA - 1, "%s", desc1[linha]);
                linha++;
            }
            matrizAberto[linhaAtualAberto + linha][0] = '8'; // marca como separador
            linhaAtualAberto += linha + 1;
        }

        // fila normal de andamento
        if (fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            // formatar string descricao (para nao ficar palavras cortadas na hora de printar)
            quebrarDescricao((*((chamado*)atualFilaAndamento->dado)).descricao, desc2);
            // guardar a descricao na matriz

            int linha = 0;
            while(desc2[linha][0] != '\0' && linha < 7) {
                matrizAndamento[linhaAtualAndamento + linha][0] = '7'; // marca como descricao
                snprintf(matrizAndamento[linhaAtualAndamento + linha] + 1, TAM_LINHA - 1, "%s", desc2[linha]);
                linha++;
            }
            matrizAndamento[linhaAtualAndamento + linha][0] = '8'; // marca como separador
            linhaAtualAndamento += linha + 1;
        }

        if (fdUsed < fdQuant && atualFilaFechado) {
            // formatar string descricao (para nao ficar palavras cortadas na hora de printar)
            quebrarDescricao((*((chamado*)atualFilaFechado->dado)).descricao, desc3);
            // guardar a descricao na matriz

            int linha = 0;
            while(desc3[linha][0] != '\0' && linha < 7) {
                matrizFechado[linhaAtualFechado + linha][0] = '7'; // marca como descricao
                snprintf(matrizFechado[linhaAtualFechado + linha] + 1, TAM_LINHA - 1, "%s", desc3[linha]);
                linha++;
            }
            matrizFechado[linhaAtualFechado + linha][0] = '8'; // marca como separador
            linhaAtualFechado += linha + 1;
        }
        // ========================================================
        // Incrementa os contadores das filas
        // ========================================================
        if (fpUsed < fpQuant) {
            fpUsed++;
        } else if (fUsed < fQuant && atualFilaAberto) {
            atualFilaAberto = atualFilaAberto->prox;
            fUsed++;
        }
        if (fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            atualFilaAndamento = atualFilaAndamento->prox;
            fAndamentoUsed++;
        }
        if (fdUsed < fdQuant && atualFilaFechado) {
            atualFilaFechado = atualFilaFechado->prox;
            fdUsed++;
        }
    }

    // ===================== CÁLCULO DE PÁGINAS =====================
    
    // cada tipo começa com 1 página
    int paginasAberto = 1, paginasAndamento = 1, paginasFechado = 1;
    int linhasNaPagina = 0;

    // ------------------ ABERTO ------------------
    linhasNaPagina = 0;
    for (int i = 0; i < fQuant + fpQuant; i++) {
        int limite = (paginasAberto == 1) ? 29 : 40;
        linhasNaPagina += linhasPorChamadoAberto[i];
        if (linhasNaPagina > limite) {
            paginasAberto++;
            linhasNaPagina = linhasPorChamadoAberto[i];
        }
    }

    // ------------------ ANDAMENTO ------------------
    linhasNaPagina = 0;
    for (int i = 0; i < fAndamentoQuant; i++) {
        int limite = (paginasAndamento == 1) ? 29 : 40;
        linhasNaPagina += linhasPorChamadoAndamento[i];
        if (linhasNaPagina > limite) {
            paginasAndamento++;
            linhasNaPagina = linhasPorChamadoAndamento[i];
        }
    }

    // ------------------ FECHADO ------------------
    linhasNaPagina = 0;
    for (int i = 0; i < fdQuant; i++) {
        int limite = (paginasFechado == 1) ? 29 : 40;
        linhasNaPagina += linhasPorChamadoFechado[i];
        if (linhasNaPagina > limite) {
            paginasFechado++;
            linhasNaPagina = linhasPorChamadoFechado[i];
        }
    }

    // número total de páginas é o máximo entre os 3
    *totalPaginas = paginasAberto;
    if (paginasAndamento > *totalPaginas) *totalPaginas = paginasAndamento;
    if (paginasFechado > *totalPaginas) *totalPaginas = paginasFechado;

    // ===================== ALOCAÇÃO DAS DIVISÕES =====================
    *divisaoLinhas = malloc(3 * sizeof(int*));
    for (int i = 0; i < 3; i++) {
        (*divisaoLinhas)[i] = calloc((*totalPaginas + 1), sizeof(int));
    }

    // ===================== PREENCHER DIVISÃO DE LINHAS =====================
    
    // ---------- ABERTO ----------
    linhasNaPagina = 0;
    paginasAberto = 1;
    int linhasAbsolutas = 0;
    (*divisaoLinhas)[0][0] = 0;
    for (int i = 0; i < (fQuant + fpQuant); i++) {
        int limite = (paginasAberto == 1) ? 29 : 40;
        linhasNaPagina += linhasPorChamadoAberto[i];
        linhasAbsolutas += linhasPorChamadoAberto[i];
        if (linhasNaPagina > limite && paginasAberto <= *totalPaginas) {
            (*divisaoLinhas)[0][paginasAberto] = linhasAbsolutas - linhasPorChamadoAberto[i];
            paginasAberto++;
            linhasNaPagina = linhasPorChamadoAberto[i];
        }
    }

    // ---------- ANDAMENTO ----------
    linhasNaPagina = 0;
    paginasAndamento = 1;
    linhasAbsolutas = 0;
    (*divisaoLinhas)[1][0] = 0;
    for (int i = 0; i < fAndamentoQuant; i++) {
        int limite = (paginasAndamento == 1) ? 29 : 40;
        linhasNaPagina += linhasPorChamadoAndamento[i];
        linhasAbsolutas += linhasPorChamadoAndamento[i];
        if (linhasNaPagina > limite && paginasAndamento <= *totalPaginas) {
            (*divisaoLinhas)[1][paginasAndamento] = linhasAbsolutas - linhasPorChamadoAndamento[i];
            paginasAndamento++;
            linhasNaPagina = linhasPorChamadoAndamento[i];
        }
    }

    // ---------- FECHADO ----------
    linhasNaPagina = 0;
    paginasFechado = 1;
    linhasAbsolutas = 0;
    (*divisaoLinhas)[2][0] = 0;
    for (int i = 0; i < fdQuant; i++) {
        int limite = (paginasFechado == 1) ? 29 : 40;
        linhasNaPagina += linhasPorChamadoFechado[i];
        linhasAbsolutas += linhasPorChamadoFechado[i];
        if (linhasNaPagina > limite && paginasFechado <= *totalPaginas) {
            (*divisaoLinhas)[2][paginasFechado] = linhasAbsolutas - linhasPorChamadoFechado[i];
            paginasFechado++;
            linhasNaPagina = linhasPorChamadoFechado[i];
        }
    }

    // ---------- COMPLETAR PÁGINAS VAZIAS ----------
    for (int i = paginasAberto; i <= *totalPaginas; i++)
        (*divisaoLinhas)[0][i] = *linhasAberto;

    for (int i = paginasAndamento; i <= *totalPaginas; i++)
        (*divisaoLinhas)[1][i] = *linhasAndamento;

    for (int i = paginasFechado; i <= *totalPaginas; i++)
        (*divisaoLinhas)[2][i] = *linhasFechado;

    

    // ===================== LIBERAR VETORES TEMPORÁRIOS =====================
    free(linhasPorChamadoAberto);
    free(linhasPorChamadoAndamento);
    free(linhasPorChamadoFechado);
    free(vetPrioridadeTemp);
}


/// @brief printar o buffer de chamados na tela, de acordo com a pagina atual
/// @param buf1 buffer de chamados abertos
/// @param buf2 buffer de chamados em andamento
/// @param buf3 buffer de chamados suspensos
/// @param linhas1 quantas linhas tem o buffer 1
/// @param linhas2 quantas linhas tem o buffer 2
/// @param linhas3 quantas linhas tem o buffer 3
/// @param paginaAtual qual a pagina atual
/// @param totalPaginas qual o total de paginas
/// @param selected qual a opcao selecionada
static void printarBufferChamados(int **divisaoLinhas, char **buf1, char **buf2, char **buf3, int linhas1, int linhas2, int linhas3, int paginaAtual, int totalPaginas, int selected){
    int maxLinhasBuffers = linhas1;
    if(linhas2 > maxLinhasBuffers) maxLinhasBuffers = linhas2;
    if(linhas3 > maxLinhasBuffers) maxLinhasBuffers = linhas3;

    if(paginaAtual == 1){
        // primeira pagina tem 11 linhas a menos (header)
        char* titulo = "Visualizar Chamados";
        repetirChar(15, '-', BLUE);
        printf(BLUE"%s"RESET, titulo);
        repetirChar(15, '-', BLUE);
        printf("\n\n");

        char* corTextoNormal = BLUE;
        char* corTextoSair = RED;
        char* fundoSelecaoNormal = BG_BLUE;
        char* fundoSelecaoSair = BG_RED;

        // printar opcoes
        char* opcoes[3] = {
            "Atender proximo chamado",
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
        // header
        printf(GREEN"+----------------------------------------------------+   "YELLOW"+----------------------------------------------------+   "RED"+----------------------------------------------------+\n"RESET);
        printf(GREEN"|                      ABERTO                        |   "YELLOW"|                   EM ANDAMENTO                     |   "RED"|                      SUSPENSO                      |\n"RESET);
        printf(GREEN"+----------------------------------------------------+   "YELLOW"+----------------------------------------------------+   "RED"+----------------------------------------------------+\n"RESET);
    }

    // ==========================================================
    // imprimir linhas do buffer dos chamados
    // ==========================================================

    
    int inicioLinhaAberto = divisaoLinhas[0][paginaAtual - 1];
    int fimLinhaAberto = divisaoLinhas[0][paginaAtual];

    int inicioLinhaAndamento = divisaoLinhas[1][paginaAtual - 1];
    int fimLinhaAndamento = divisaoLinhas[1][paginaAtual];

    int inicioLinhaFechado = divisaoLinhas[2][paginaAtual - 1];
    int fimLinhaFechado = divisaoLinhas[2][paginaAtual];

    
    int fimLoop = (paginaAtual == 1) ? 40 - 11 : 40; // primeira pagina tem 11 linhas a menos (header)
    for(int i = 0; i < fimLoop; i++){

        // imprimir linha
        char firstCharAberto;
        char firstCharAndamento;
        char firstCharFechado;

        int i1 = i + inicioLinhaAberto;
        int i2 = i + inicioLinhaAndamento;
        int i3 = i + inicioLinhaFechado;

        if(i1 >= fimLinhaAberto || i1 >= linhas1){
            firstCharAberto = ' ';
        } else {
            firstCharAberto = buf1[i1][0];
        }

        if(i2 >= fimLinhaAndamento || i2 >= linhas2){
            firstCharAndamento = ' ';
        } else {
            firstCharAndamento = buf2[i2][0];
        }

        if(i3 >= fimLinhaFechado || i3 >= linhas3){
            firstCharFechado = ' ';
        } else {
            firstCharFechado = buf3[i3][0];
        }
        // ---------------------------------------------------------
        
        switch (firstCharAberto){
            case '0':
                // linha vazia
                printf(GREEN"| %-50s |"RESET, "");
                break;
            case '1':
                // titulo
                printf("%s| %s%-50s %s|"RESET, GREEN, BLUE, buf1[i1] + 1, GREEN);
                break;

            case '2':
                // prioridade
                printf("%s| %sPrioridade: %s%-38s %s|"RESET, GREEN, MAGENTA, WHITE, buf1[i1] + 1, GREEN);
                break;
            case '3':
                // criador
                printf("%s| %sCriador: %s%-41s %s|"RESET, GREEN, MAGENTA, WHITE, buf1[i1] + 1, GREEN);
                break;
            case 4:
                // atendente
                printf("%s| %sAtendente: %s%-39s %s|"RESET, GREEN, MAGENTA, WHITE, buf1[i1] + 1, GREEN);
                break;
            case '5':
                // data e hora
                printf("%s| %sData e Hora: %s%-37s %s|"RESET, GREEN, MAGENTA, WHITE, buf1[i1] + 1, GREEN);
                break;
            case '6':
                // materiais
                printf("%s| %s%-50s %s|"RESET, GREEN, WHITE, buf1[i1] + 1, GREEN);
                break;
            case '7':
                // descricao
                printf("%s| %s%-50s %s|"RESET, GREEN, CYAN, buf1[i1] + 1, GREEN);
                break;
            case '8':
                // separador
                printf(GREEN"+----------------------------------------------------+"RESET);
                break;
            default:
                // linha vazia
                printf("%-54s", "");
                break;
        }
        printf("   ");

        // ---------------------------------------------------------
        switch (firstCharAndamento)
        {
            case '0':
                // linha vazia
                printf(YELLOW"| %-50s |"RESET, "");
                break;
            case '1':
                // titulo
                printf("%s| %s%-50s %s|"RESET, YELLOW, BLUE, buf1[i1] + 1, YELLOW);
                break;

            case '2':
                // prioridade
                printf("%s| %sPrioridade: %s%-38s %s|"RESET, YELLOW, MAGENTA, WHITE, buf1[i1] + 1, YELLOW);
                break;
            case '3':
                // criador
                printf("%s| %sCriador: %s%-41s %s|"RESET, YELLOW, MAGENTA, WHITE, buf1[i1] + 1, YELLOW);
                break;
            case 4:
                // atendente
                printf("%s| %sAtendente: %s%-39s %s|"RESET, YELLOW, MAGENTA, WHITE, buf1[i1] + 1, YELLOW);
                break;
            case '5':
                // data e hora
                printf("%s| %sData e Hora: %s%-37s %s|"RESET, YELLOW, MAGENTA, WHITE, buf1[i1] + 1, YELLOW);
                break;
            case '6':
                // materiais
                printf("%s| %s%-50s %s|"RESET, YELLOW, WHITE, buf1[i1] + 1, YELLOW);
                break;
            case '7':
                // descricao
                printf("%s| %s%-50s %s|"RESET, YELLOW, CYAN, buf1[i1] + 1, YELLOW);
                break;
            case '8':
                // separador
                printf(YELLOW"+----------------------------------------------------+"RESET);
                break;
            default:
                // linha vazia
                printf("%-54s", "");
                break;
        }
        printf("   ");

        // ---------------------------------------------------------
        switch (firstCharFechado)
        {
            case '0':
                // linha vazia
                printf(RED"| %-50s |"RESET, "");
                break;
            case '1':
                // titulo
                printf("%s| %s%-50s %s|"RESET, RED, BLUE, buf1[i1] + 1, RED);
                break;

            case '2':
                // prioridade
                printf("%s| %sPrioridade: %s%-38s %s|"RESET, RED, MAGENTA, WHITE, buf1[i1] + 1, RED);
                break;
            case '3':
                // criador
                printf("%s| %sCriador: %s%-41s %s|"RESET, RED, MAGENTA, WHITE, buf1[i1] + 1, RED);
                break;
            case 4:
                // atendente
                printf("%s| %sAtendente: %s%-39s %s|"RESET, RED, MAGENTA, WHITE, buf1[i1] + 1, RED);
                break;
            case '5':
                // data e hora
                printf("%s| %sData e Hora: %s%-37s %s|"RESET, RED, MAGENTA, WHITE, buf1[i1] + 1, RED);
                break;
            case '6':
                // materiais
                printf("%s| %s%-50s %s|"RESET, RED, WHITE, buf1[i1] + 1, RED);
                break;
            case '7':
                // descricao
                printf("%s| %s%-50s %s|"RESET, RED, CYAN, buf1[i1] + 1, RED);
                break;
            case '8':
                // separador
                printf(RED"+----------------------------------------------------+"RESET);
                break;
            default:
                // linha vazia
                printf("%-54s", "");
                break;
        }
        printf("\n");
        fflush(stdout);
    }
    printf("\n");
    if(selected == 4){
        // pagina selecionada
        printf(BG_BLUE" <- Pagina %d/%d ->"RESET, paginaAtual, totalPaginas);
    } else {
        printf(BLUE" <- Pagina %d/%d ->"RESET, paginaAtual, totalPaginas);
    }
    fflush(stdout);
}

/// @brief menu para visualizar chamados
void chamadosAdmins(void){
    char* opcoes[3] = {
            "Atender proximo chamado",
            "Voltar",
            "Sair"
        };

    int paginaAtual = 1;
    int selected = 1;
    char **matrizAberto = NULL;
    char **matrizAndamento = NULL;
    char **matrizFechado = NULL;

    int linhasAberto = 0;
    int linhasAndamento = 0;
    int linhasFechado = 0;
    int totalPaginas = 0;

    int** divisaoLinhas = NULL;
    esperar_tamanho_minimo(42, 170);
    if(terminalPequenoAlertado){
        
        terminalPequenoAlertado = 0;
    }
    clear();
    carregarBufferChamados(&matrizAberto, &matrizAndamento, &matrizFechado, &linhasAberto, &linhasAndamento, &linhasFechado, &divisaoLinhas, &totalPaginas);
    printarBufferChamados(divisaoLinhas, matrizAberto, matrizAndamento, matrizFechado, linhasAberto, linhasAndamento, linhasFechado, paginaAtual, totalPaginas, selected);

    while(1){
        KeyCode k = userGetKey();
        if(k == RESIZE_EVENT){
            esperar_tamanho_minimo(42, 170);
            if(terminalPequenoAlertado){
                clear();
                printarBufferChamados(divisaoLinhas, matrizAberto, matrizAndamento, matrizFechado, linhasAberto, linhasAndamento, linhasFechado, paginaAtual, totalPaginas, selected);
                terminalPequenoAlertado = 0;
            }
            continue;
        }else if(k == KC_ESC){
            menuHandler* temp = (menuHandler*)popPilha(estruturasGlobais.pil); // tira o menu de atender chamado
            free(temp); // tira o menu de atender chamado
            break;
        }
        if(paginaAtual == 1){
           if(k == KC_DOWN){
                if(selected < 4){  // máximo 4 opções
                    // desmarcar opção atual
                    updateOption(selected + 2, opcoes[selected-1], "", (selected == 3 ? RED : BLUE));
                    selected++;
                    // marcar opção nova
                    if(selected == 4){
                        // pagina
                        char fimPaginaStr[20];
                        snprintf(fimPaginaStr, 20, "<- Pagina %d/%d ->", paginaAtual, totalPaginas);
                        updateOption(42, fimPaginaStr, BG_BLUE, "");
                    } else {
                        updateOption(selected + 2, opcoes[selected-1], (selected == 5 ? BG_RED : BG_BLUE), "");
                    }
                }
            }else if(k == KC_UP){
                if(selected > 1){
                    // desmarcar opção atual
                    if(selected == 4){
                        char fimPaginaStr[20];
                        snprintf(fimPaginaStr, 20, "<- Pagina %d/%d ->", paginaAtual, totalPaginas);
                        updateOption(42, fimPaginaStr, "", BLUE);
                    } else {
                        updateOption(selected + 2, opcoes[selected-1], "", (selected == 3 ? RED : BLUE));
                    }
                    selected--;
                    // marcar opção nova
                    updateOption(selected + 2, opcoes[selected-1], (selected == 3 ? BG_RED : BG_BLUE), "");
                }
            
            }else if(k == KC_RIGHT && selected == 4){
                if(paginaAtual < totalPaginas){
                    paginaAtual++;
                    clear();
                    printarBufferChamados(divisaoLinhas, matrizAberto, matrizAndamento, matrizFechado, linhasAberto, linhasAndamento, linhasFechado, paginaAtual, totalPaginas, selected);
                }
            }else if(k == KC_LEFT && selected == 4){
                if(paginaAtual > 1){
                    paginaAtual--;
                    clear();
                    printarBufferChamados(divisaoLinhas, matrizAberto, matrizAndamento, matrizFechado, linhasAberto, linhasAndamento, linhasFechado, paginaAtual, totalPaginas, selected);
                }
            }else if(k == KC_ENTER && selected < 4){
                if(selected == 1){
                    // atendender proximo chamado
                    if(linhasAberto == 0){
                        // nao ha chamados abertos
                        menuHandler* m = createMenu(12);
                        pushPilha(estruturasGlobais.pil, m);
                        break;
                    } else {
                        menuHandler* m = createMenu(13);
                        pushPilha(estruturasGlobais.pil, m);
                        break;
                    }
                }else if(selected == 2){
                    // voltar
                    menuHandler* temp = (menuHandler*)popPilha(estruturasGlobais.pil); // tira o menu de atender chamado
                    free(temp); // tira o menu de atender chamado
                    break;
                }else if(selected == 3){
                    // sair
                    menuHandler* m = createMenu(9);
                    pushPilha(estruturasGlobais.pil, m);
                    break;
                }
            }
        }else{
            if(k == KC_RIGHT){
                if(paginaAtual < totalPaginas){
                    paginaAtual++;
                    clear();
                    printarBufferChamados(divisaoLinhas, matrizAberto, matrizAndamento, matrizFechado, linhasAberto, linhasAndamento, linhasFechado, paginaAtual, totalPaginas, selected);
                }
            }else if(k == KC_LEFT){
                if(paginaAtual > 1){
                    paginaAtual--;
                    clear();
                    printarBufferChamados(divisaoLinhas, matrizAberto, matrizAndamento, matrizFechado, linhasAberto, linhasAndamento, linhasFechado, paginaAtual, totalPaginas, selected);
                }
            }
        }
    }
    
     // liberar memoria
    if(linhasAberto > 0){
        for (int i = 0; i < linhasAberto; i++) {
            free(matrizAberto[i]);
        }
        free(matrizAberto);
    }
    if(linhasAndamento > 0){
        for (int i = 0; i < linhasAndamento; i++) {
            free(matrizAndamento[i]);
        }
        free(matrizAndamento);
    }
    if(linhasFechado > 0){
        for (int i = 0; i < linhasFechado; i++) {
            free(matrizFechado[i]);
        }
        free(matrizFechado);
    }

    if(divisaoLinhas != NULL){
        for(int i = 0; i < 3; i++){
            free(divisaoLinhas[i]);
        }
        free(divisaoLinhas);
    }
}