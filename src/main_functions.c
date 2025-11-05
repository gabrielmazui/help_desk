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

#define TAM_LINHA 52

// =================================================================
// Funcoes auxiliares
// =================================================================

/// @brief Cria um struct com os dados do tempo atual
/// @return Um struct Tempo com os dados do tempo atual
static Tempo tempoAgora(void){
    Tempo t;

    time_t agora = time(NULL);           // pega o tempo atual em segundos
    struct tm *info = localtime(&agora); // converte para data/hora local

    // copia para a struct personalizada
    t.segundos = info->tm_sec;
    t.minutos  = info->tm_min;
    t.horas    = info->tm_hour;
    t.dia      = info->tm_mday;
    t.mes      = info->tm_mon + 1;     // +1 porque tm_mon vai de 0 a 11
    t.ano      = info->tm_year + 1900; // +1900 porque tm_year conta desde 1900
    return t;
}

/// @brief marcar ou desmarcar um chamado no menu de concluir chamado em andamento
/// @param paginaAtual pagina atual
/// @param selected chamado selecionado
/// @param divisaoLinhas divisao de linhas
/// @param marcar 1 para marcar, 0 para desmarcar
static void updateChamadoAndamento(int paginaAtual, int selected, int ** divisaoLinhas, int ** divisaoPaginas, int marcar){
    int linhaInicio = divisaoLinhas[paginaAtual-1][selected - 1];
    if(linhaInicio == 0) linhaInicio = 1; // chamado nao existe
    printf("\033[%d;1H\033[K", linhaInicio); // move para linha e coluna 1 e limpa a linha
    fila* fAndamento = estruturasGlobais.filaAndamento;    
    noFila *no = fAndamento->first;
    chamado cham;

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
    cham = no->chamado;

    char* color = marcar ? BLUE : YELLOW;
    // printar chamado
    printf("%s+----------------------------------------------------+\n"RESET, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %s%-50s %s|\n"RESET, color, BLUE, cham.titulo, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %sPrioridade: %s%-38d %s|\n", color, MAGENTA, WHITE, cham.prioridade, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %sCriador: %s%-41s %s|\n"RESET, color, MAGENTA, WHITE, cham.criador, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
    color, MAGENTA, WHITE,
    cham.tempoComplexo.dia, cham.tempoComplexo.mes, cham.tempoComplexo.ano,
    cham.tempoComplexo.horas, cham.tempoComplexo.minutos, cham.tempoComplexo.segundos,
    "", color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %-50s %s|\n"RESET, color, "", color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    char descricao[7][50] = {0};
    int linhas = quebrarDescricao(cham.descricao, descricao);
    // descricao
    for(int i = 0; i < linhas; i++){
        printf("%s| %s%-50s %s|\n"RESET, color, CYAN, descricao[i], color);
        printf("\033[%d;1H\033[K", ++linhaInicio);
    }
    // separador
    printf("%s+----------------------------------------------------+"RESET, color);
    
}

/// @brief marcar ou desmarcar um chamado no menu de deletar chamado fechado
/// @param paginaAtual pagina atual
/// @param selected chamado selecionado
/// @param divisaoLinhas divisao de linhas
/// @param marcar 1 para marcar, 0 para desmarcar
static void updateChamadoFechado(int paginaAtual, int selected, int ** divisaoLinhas, int ** divisaoPaginas, int marcar){
    int linhaInicio = divisaoLinhas[paginaAtual-1][selected - 1];
    if(linhaInicio == 0) linhaInicio = 1; // chamado nao existe
    printf("\033[%d;1H\033[K", linhaInicio); // move para linha e coluna 1 e limpa a linha
    filaDupla* fd = estruturasGlobais.filadupla;    
    noDuplo *no = fd->inicio;
    chamado cham;

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
    cham = no->chamado;
    char* color = marcar ? BLUE : RED;
    // printar chamado
    printf("%s+----------------------------------------------------+\n"RESET, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %s%-50s %s|\n"RESET, color, BLUE, cham.titulo, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %sPrioridade: %s%-38d %s|\n", color, MAGENTA, WHITE, cham.prioridade, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %sCriador: %s%-41s %s|\n"RESET, color, MAGENTA, WHITE, cham.criador, color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
    color, MAGENTA, WHITE,
    cham.tempoComplexo.dia, cham.tempoComplexo.mes, cham.tempoComplexo.ano,
    cham.tempoComplexo.horas, cham.tempoComplexo.minutos, cham.tempoComplexo.segundos,
    "", color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    printf("%s| %-50s %s|\n"RESET, color, "", color);
    printf("\033[%d;1H\033[K", ++linhaInicio);

    char descricao[7][50] = {0};
    int linhas = quebrarDescricao(cham.descricao, descricao);
    // descricao
    for(int i = 0; i < linhas; i++){
        printf("%s| %s%-50s %s|\n"RESET, color, CYAN, descricao[i], color);
        printf("\033[%d;1H\033[K", ++linhaInicio);
    }
    // separador
    printf("%s+----------------------------------------------------+"RESET, color);
    
}


/// @brief  Cria um vetor ordenado dos chamados na fila de prioridade
/// @param fp Ponteiro para a fila de prioridade
/// @return Um vetor de chamados ordenados
static chamado* vetorOrdenadoFilaPrioridade(filaPrioridade* fp) {
    filaPrioridade* fpCopia = filaPrioridadeCopiar(fp);
    // para conseguir pegar o topo sempre e colocar em um vetor
    chamado* vetor = NULL;
    if(fp->n > 0){
        vetor = (chamado*)malloc(fp->n * sizeof(chamado));
    }
    // colocar em ordem no vetor
    for(int i = 0; i < fp->n; i++){
        vetor[i] = filaPrioridadeRemover(fpCopia);
    }
    filaPrioridadeLiberar(&fpCopia);
    return vetor;
}

// =================================================================
// funcao principal do menu criar chamado
// ================================================================


/// @brief Imprime o menu para criar um chamado
/// @param type O tipo de pergunta a ser exibida
/// @param entradaErrada Indica se a entrada anterior foi inválida
static void criarChamadoPrintMenu(int type, int entradaErrada){
    char* pergunta1 = "Qual a prioridade desse chamado?";
    char* pergunta2 = "Qual o titulo do chamado?";
    char* pergunta3 = "Descreva melhor o seu problema:";
    switch(type){
        // 
        case 1:
            repetirChar(15, '-', BLUE);
            printf(BLUE"%s"RESET, "Criar chamado");
            repetirChar(15, '-', BLUE);
            printf("\n");
            if(entradaErrada){
                printf(RED" Escolha um numero maior que 0"RESET);
                printf("\n");
            }
            printf(GREEN" %s"RESET, pergunta1);
            printf("\n\n ");
            fflush(stdout);
            break;
        case 2:
            repetirChar(15, '-', BLUE);
            printf(BLUE"%s"RESET, "Criar chamado");
            repetirChar(15, '-', BLUE);
            printf("\n");
            
            printf(GREEN" %s"RESET, pergunta2);
            printf("\n\n ");
            fflush(stdout);
            break;
        case 3:
            repetirChar(15, '-', BLUE);
            printf(BLUE"%s"RESET, "Criar chamado");
            repetirChar(15, '-', BLUE);
            printf("\n");
            
            printf(GREEN" %s"RESET, pergunta3);
            printf("\n\n ");
            fflush(stdout);
            break;
    }
}

/// @brief Função para o menu de criar um chamado
/// @param prioridade A prioridade do chamado
/// @param comPreferencia Indica se o chamado tem preferência
/// @param user O usuário que está criando o chamado
/// @param titulo O título do chamado
/// @param descricao A descrição do chamado
/// @param pularPrioridade Indica se deve pular a pergunta de prioridade
void menuCriarChamadoHandler(int prioridade, int comPreferencia, char* user, char titulo[50], char descricao[350], int pularPrioridade){
    int entradaErrada = 0;
    // descobre a prioridade , caso tiver
    int mostrarBuffer = 1;
    if(comPreferencia == 1 && !pularPrioridade){
        esperar_tamanho_minimo(10, 45);
        clear();
        criarChamadoPrintMenu(1, entradaErrada);
        char digitos[7];
        if(prioridade == 0){
            digitos[0] = '\0';
        }else{
            sprintf(digitos, "%d", prioridade);
        }
        while(1){
            
            // le o input de numero
            KeyCode k = inputNumeroASCII(digitos, sizeof(digitos) - 1, WHITE, mostrarBuffer);
            if(k == RESIZE_EVENT){
                esperar_tamanho_minimo(10, 45);
                if(terminalPequenoAlertado){
                    terminalPequenoAlertado = 0;
                    clear();
                    criarChamadoPrintMenu(1, entradaErrada);
                    mostrarBuffer = 1;
                    continue;
                }
                mostrarBuffer = 0;
                continue;
            }
            if(k == KC_ESC){
                // se tiver esc volta pro menu de pergunta se tem prioridade
                
                popPilha(estruturasGlobais.pil);
                clear();
                return;
            }
            // transforma o vetor de char de digitos em numero    
            prioridade = atoi(digitos);
            
            if(prioridade <= 0){
                // continua o loop
                clear();
                entradaErrada = 1;
                criarChamadoPrintMenu(1, entradaErrada);
            }else{
                clear();
                break;
            }
        }
    }

    // parte 2
    // pergunta o titulo da chamada
    esperar_tamanho_minimo(10, 55);
    clear();
    criarChamadoPrintMenu(2, 0);
    mostrarBuffer = 1;
    while(1){
        KeyCode k2 = inputASCII(titulo, 40, WHITE , mostrarBuffer); // ler input
        // funcao para verificar se e uma string valida
        if(k2 == RESIZE_EVENT){
            esperar_tamanho_minimo(10, 55);
            if(terminalPequenoAlertado){
                terminalPequenoAlertado = 0;
                clear();
                criarChamadoPrintMenu(2, 0);
                mostrarBuffer = 1;
                continue;
            }
            mostrarBuffer = 0;
            continue;
        }else if(k2 == KC_ESC){
            // se for pressionado o esc retorna para o primeiro menu
            clear();
            if(comPreferencia){
                // preferencia 0 para permitir que seja perguntado de novo
                menuCriarChamadoHandler(prioridade, comPreferencia, user, titulo, descricao, 0);
            }else{
                popPilha(estruturasGlobais.pil);
                // caso nao tiver preferencia, volta para o menu de criar chamado
            }
            // chamo a funcao novamente, para voltar ao menu de qual a preferencia
            // caso apertar o esc denovo, ele voltar para o menu de criar chamado original
            return;
        }
        if(strlen(titulo) == 0){
            continue;
        }
        break;
    }
    clear();
    
    // descrever o problema

    esperar_tamanho_minimo(13, 55);
    clear();
    criarChamadoPrintMenu(3, 0);
    mostrarBuffer = 1;
    while(1){
        KeyCode k3 = inputASCII(descricao, 300, WHITE, mostrarBuffer); // ler input

        if(k3 == RESIZE_EVENT){
            esperar_tamanho_minimo(13, 55);
            if(terminalPequenoAlertado){
                terminalPequenoAlertado = 0;
                clear();
                criarChamadoPrintMenu(3, 0);
                mostrarBuffer = 1;
                continue;
            }
            mostrarBuffer = 0;
            continue;
        }else if(k3 == KC_ESC){
            // se for pressionado o esc retorna para o primeiro menu
            clear();
            menuCriarChamadoHandler(prioridade, comPreferencia, user, titulo, descricao, 1);
            return;
        }
        if(strlen(descricao) == 0){
            // descricao invalida
            continue;
        }
        break;
    }
    clear();


    chamado cham;
    cham.status = 1;
    
    strncpy(cham.titulo, titulo, sizeof(cham.titulo)-1);
    cham.titulo[sizeof(cham.titulo)-1] = '\0'; // garante terminação

    strncpy(cham.descricao, descricao, sizeof(cham.descricao)-1);
    cham.descricao[sizeof(cham.descricao)-1] = '\0';

    strncpy(cham.criador, user, sizeof(cham.criador)-1);
    cham.criador[sizeof(cham.criador)-1] = '\0';

    Tempo t = tempoAgora();
    time_t agora = time(NULL);          // pega o tempo atual em segundos
    cham.tempoSimples = agora;
    cham.tempoComplexo = t;
    
    if(comPreferencia == 1){
        cham.prioridade = prioridade;
    }else{
        cham.prioridade = 0;
    }
    popPilha(estruturasGlobais.pil);
    popPilha(estruturasGlobais.pil);
    criarChamado(cham);
}

// =================================================================
// menu visualizar chamados
// ================================================================

/// @brief Carrega os buffers de chamados para exibição
/// @param buf1 buffer de chamados abertos
/// @param buf2 buffer de chamados em andamento
/// @param buf3 buffer de chamados fechados
/// @param linhasAberto número de linhas no buffer de chamados abertos
/// @param linhasAndamento número de linhas no buffer de chamados em andamento
/// @param linhasFechado número de linhas no buffer de chamados fechados
/// @param divisaoLinhas matriz de divisao de linhas por pagina de cada buffer
/// @param totalPaginas número total de páginas necessárias para exibição
static void carregarBufferChamados(char ***buf1, char ***buf2, char ***buf3, int* linhasAberto, int* linhasAndamento, int* linhasFechado, int*** divisaoLinhas, int* totalPaginas) {
    
    fila *f = estruturasGlobais.filaNormal;
    filaPrioridade *fp = estruturasGlobais.filaPrioridade;
    filaDupla *fd = estruturasGlobais.filadupla;
    fila *fAndamento = estruturasGlobais.filaAndamento;

    noFila *atualFilaAberto = f ? f->first : NULL;
    noDuplo *atualFilaFechado = fd ? fd->inicio : NULL;
    noFila *atualFilaAndamento = fAndamento ? fAndamento->first : NULL;

   chamado* vetPrioridadeTemp = vetorOrdenadoFilaPrioridade(fp);

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
                int linhasDescricao = quebrarDescricao(vetPrioridadeTemp[i].descricao, tempDestino);
                int linhasTotais = linhasDescricao + 6;

                linhasPorChamadoAberto[index++] = linhasTotais;
                totalLinhasAberto += linhasTotais;
            }
        }

        // fila normal
        noFila *atual = f ? f->first : NULL;
        while (atual) {
            char tempDestino[7][50];
            int linhasDescricao = quebrarDescricao(atual->chamado.descricao, tempDestino);
            int linhasTotais = linhasDescricao + 6;

            linhasPorChamadoAberto[index++] = linhasTotais;
            totalLinhasAberto += linhasTotais;

            atual = atual->prox;
        }

    }
    
    // ----------- Andamento -----------
    {
        int index = 0;
        noFila *atual = fAndamento ? fAndamento->first : NULL;
        while (atual) {
            char tempDestino[7][50];
            int linhasDescricao = quebrarDescricao(atual->chamado.descricao, tempDestino);
            int linhasTotais = linhasDescricao + 6;

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
            int linhasDescricao = quebrarDescricao(atual->chamado.descricao, tempDestino);
            int linhasTotais = linhasDescricao + 6;

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
        if (!(*buf1)) { // logs
            perror("calloc matrizAberto"); exit(1); 
        }
        for (int i = 0; i < *linhasAberto; i++) {
            (*buf1)[i] = calloc(52, sizeof(char)); // 51 + '\0'
            if (!(*buf1)[i]) { 
                // logs
                perror("calloc matrizAberto[i]"); exit(1); 
            }
        }
    }

    // alocar matrizAndamento se tiver linhas
    if (*linhasAndamento > 0) {
        *buf2 = calloc(*linhasAndamento, sizeof(char*));
        if (!*buf2) { 
            // logs
            perror("calloc matrizAndamento"); exit(1); 
        }
        for (int i = 0; i < *linhasAndamento; i++) {
            (*buf2)[i] = calloc(52, sizeof(char));
            if (!(*buf2)[i]) { 
                // logs
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
    // 4 - data e hora
    // 5 - descricao
    // 6 - separador

    
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
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s", vetPrioridadeTemp[fpUsed].titulo);
            linhaAtualAberto++;
        } else if (fUsed < fQuant && atualFilaAberto) {
            matrizAberto[linhaAtualAberto][0] = '1'; // marca como titulo
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s", atualFilaAberto->chamado.titulo);
            linhaAtualAberto++;
        }
        if (fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '1'; // marca como titulo
            snprintf(matrizAndamento[linhaAtualAndamento] + 1, TAM_LINHA - 1, "%s", atualFilaAndamento->chamado.titulo);
            linhaAtualAndamento++;
        }
        
        fflush(stdout);
        if (fdUsed < fdQuant && atualFilaFechado) {
            matrizFechado[linhaAtualFechado][0] = '1'; // marca como titulo
            snprintf(matrizFechado[linhaAtualFechado] + 1, TAM_LINHA - 1, "%s", atualFilaFechado->chamado.titulo);
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
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%d", vetPrioridadeTemp[fpUsed].prioridade);
            linhaAtualAberto++;
        } else if (fUsed < fQuant && atualFilaAberto) {
            matrizAberto[linhaAtualAberto][0] = '2'; // marca como prioridade
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%d", atualFilaAberto->chamado.prioridade);
            linhaAtualAberto++;
        }

        if (fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '2'; // marca como prioridade
            snprintf(matrizAndamento[linhaAtualAndamento] + 1, TAM_LINHA - 1, "%d", atualFilaAndamento->chamado.prioridade);
            linhaAtualAndamento++;
        }

        if (fdUsed < fdQuant && atualFilaFechado) {
            matrizFechado[linhaAtualFechado][0] = '2'; // marca como prioridade
            snprintf(matrizFechado[linhaAtualFechado] + 1, TAM_LINHA - 1, "%d", atualFilaFechado->chamado.prioridade);
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
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s", vetPrioridadeTemp[fpUsed].criador);
            linhaAtualAberto++;
        } else if (fUsed < fQuant && atualFilaAberto) {
            matrizAberto[linhaAtualAberto][0] = '3'; // marca como criador
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s", atualFilaAberto->chamado.criador);
            linhaAtualAberto++;
        }
        if (fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '3'; // marca como criador
            snprintf(matrizAndamento[linhaAtualAndamento] + 1, TAM_LINHA - 1, "%s", atualFilaAndamento->chamado.criador);
            linhaAtualAndamento++;
        }

        if (fdUsed < fdQuant && atualFilaFechado) {
            matrizFechado[linhaAtualFechado][0] = '3'; // marca como criador
            snprintf(matrizFechado[linhaAtualFechado] + 1, TAM_LINHA - 1, "%s", atualFilaFechado->chamado.criador);
            linhaAtualFechado++;
        }
        
        
        // ========================================================
        // Linha 4: DATA E HORA
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
            matrizAberto[linhaAtualAberto][0] = '4'; // marca como data e hora
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%02d/%02d/%04d %02d:%02d:%02d",
                    vetPrioridadeTemp[fpUsed].tempoComplexo.dia,
                    vetPrioridadeTemp[fpUsed].tempoComplexo.mes,
                    vetPrioridadeTemp[fpUsed].tempoComplexo.ano,
                    vetPrioridadeTemp[fpUsed].tempoComplexo.horas,
                    vetPrioridadeTemp[fpUsed].tempoComplexo.minutos,
                    vetPrioridadeTemp[fpUsed].tempoComplexo.segundos);
            linhaAtualAberto++;
        }
        else if (fUsed < fQuant && atualFilaAberto) {
            matrizAberto[linhaAtualAberto][0] = '4'; // marca como data e hora
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%02d/%02d/%04d %02d:%02d:%02d",
                    atualFilaAberto->chamado.tempoComplexo.dia,
                    atualFilaAberto->chamado.tempoComplexo.mes,
                    atualFilaAberto->chamado.tempoComplexo.ano,
                    atualFilaAberto->chamado.tempoComplexo.horas,
                    atualFilaAberto->chamado.tempoComplexo.minutos,
                    atualFilaAberto->chamado.tempoComplexo.segundos);
            linhaAtualAberto++;
        }
        if (fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            matrizAndamento[linhaAtualAndamento][0] = '4'; // marca como data e hora
            snprintf(matrizAndamento[linhaAtualAndamento] + 1, TAM_LINHA - 1, "%02d/%02d/%04d %02d:%02d:%02d",
                    atualFilaAndamento->chamado.tempoComplexo.dia,
                    atualFilaAndamento->chamado.tempoComplexo.mes,
                    atualFilaAndamento->chamado.tempoComplexo.ano,
                    atualFilaAndamento->chamado.tempoComplexo.horas,
                    atualFilaAndamento->chamado.tempoComplexo.minutos,
                    atualFilaAndamento->chamado.tempoComplexo.segundos);
            linhaAtualAndamento++;
        }
        
        if (fdUsed < fdQuant && atualFilaFechado) {
            matrizFechado[linhaAtualFechado][0] = '4'; // marca como data e hora
            snprintf(matrizFechado[linhaAtualFechado] + 1, TAM_LINHA - 1, "%02d/%02d/%04d %02d:%02d:%02d",
                    atualFilaFechado->chamado.tempoComplexo.dia,
                    atualFilaFechado->chamado.tempoComplexo.mes,
                    atualFilaFechado->chamado.tempoComplexo.ano,
                    atualFilaFechado->chamado.tempoComplexo.horas,
                    atualFilaFechado->chamado.tempoComplexo.minutos,
                    atualFilaFechado->chamado.tempoComplexo.segundos);
            linhaAtualFechado++;
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
        // Linha 5+: DESCRIÇÃO (7 linhas max)
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
            quebrarDescricao(vetPrioridadeTemp[fpUsed].descricao, desc1);
            // guardar a descricao na matriz
            int linha = 0;
            while(desc1[linha][0] != '\0' && linha < 7) {
                matrizAberto[linhaAtualAberto + linha][0] = '5'; // marca como descricao
                snprintf(matrizAberto[linhaAtualAberto + linha] + 1, TAM_LINHA - 1, "%s", desc1[linha]);
                linha++;
            }
            matrizAberto[linhaAtualAberto + linha][0] = '6'; // marca como separador
            linhaAtualAberto += linha + 1;

        // fila normal de aberto
        } else if (fUsed < fQuant && atualFilaAberto) {
            // formatar string descricao (para nao ficar palavras cortadas na hora de printar)
            quebrarDescricao(atualFilaAberto->chamado.descricao, desc1);
            // guardar a descricao na matriz
            int linha = 0;
            while(desc1[linha][0] != '\0' && linha < 7) {
                matrizAberto[linhaAtualAberto + linha][0] = '5'; // marca como descricao
                snprintf(matrizAberto[linhaAtualAberto + linha] + 1, TAM_LINHA - 1, "%s", desc1[linha]);
                linha++;
            }
            matrizAberto[linhaAtualAberto + linha][0] = '6'; // marca como separador
            linhaAtualAberto += linha + 1;
        }

        // fila normal de andamento
        if (fAndamentoUsed < fAndamentoQuant && atualFilaAndamento) {
            // formatar string descricao (para nao ficar palavras cortadas na hora de printar)
            quebrarDescricao(atualFilaAndamento->chamado.descricao, desc2);
            // guardar a descricao na matriz

            int linha = 0;
            while(desc2[linha][0] != '\0' && linha < 7) {
                matrizAndamento[linhaAtualAndamento + linha][0] = '5'; // marca como descricao
                snprintf(matrizAndamento[linhaAtualAndamento + linha] + 1, TAM_LINHA - 1, "%s", desc2[linha]);
                linha++;
            }
            matrizAndamento[linhaAtualAndamento + linha][0] = '6'; // marca como separador
            linhaAtualAndamento += linha + 1;
        }

        if (fdUsed < fdQuant && atualFilaFechado) {
            // formatar string descricao (para nao ficar palavras cortadas na hora de printar)
            quebrarDescricao(atualFilaFechado->chamado.descricao, desc3);
            // guardar a descricao na matriz

            int linha = 0;
            while(desc3[linha][0] != '\0' && linha < 7) {
                matrizFechado[linhaAtualFechado + linha][0] = '5'; // marca como descricao
                snprintf(matrizFechado[linhaAtualFechado + linha] + 1, TAM_LINHA - 1, "%s", desc3[linha]);
                linha++;
            }
            matrizFechado[linhaAtualFechado + linha][0] = '6'; // marca como separador
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
        int limite = (paginasAberto == 1) ? 27 : 40;
        linhasNaPagina += linhasPorChamadoAberto[i];
        if (linhasNaPagina > limite) {
            paginasAberto++;
            linhasNaPagina = linhasPorChamadoAberto[i];
        }
    }

    // ------------------ ANDAMENTO ------------------
    linhasNaPagina = 0;
    for (int i = 0; i < fAndamentoQuant; i++) {
        int limite = (paginasAndamento == 1) ? 27 : 40;
        linhasNaPagina += linhasPorChamadoAndamento[i];
        if (linhasNaPagina > limite) {
            paginasAndamento++;
            linhasNaPagina = linhasPorChamadoAndamento[i];
        }
    }

    // ------------------ FECHADO ------------------
    linhasNaPagina = 0;
    for (int i = 0; i < fdQuant; i++) {
        int limite = (paginasFechado == 1) ? 27 : 40;
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
        int limite = (paginasAberto == 1) ? 27 : 40;
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
        int limite = (paginasAndamento == 1) ? 27 : 40;
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
        int limite = (paginasFechado == 1) ? 27 : 40;
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
/// @param buf3 buffer de chamados fechados
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

    int totalLinhas = maxLinhasBuffers + 13;

    if(paginaAtual == 1){
        // primeira pagina tem 13 linhas a menos (header)
        char* titulo = "Visualizar Chamados";
        repetirChar(15, '-', BLUE);
        printf(BLUE"%s"RESET, titulo);
        repetirChar(15, '-', BLUE);
        printf("\n\n");

        /*
        * Atender proximo chamado - 1
        * Concluir chamado em andamento - 2
        * Deletar chamado fechado - 3
        * Voltar - 4
        * Sair - 5
        * Pagina X de Y - 6 (ULTIMA LINHA)
        */

        char* corTextoNormal = BLUE;
        char* corTextoSair = RED;
        char* fundoSelecaoNormal = BG_BLUE;
        char* fundoSelecaoSair = BG_RED;

        // printar opcoes
        char* opcoes[5] = {
            "Atender proximo chamado",
            "Concluir chamado em andamento",
            "Deletar chamado fechado",
            "Voltar",
            "Sair"
        };
        for(int i = 1; i <= 5; i++){
            if(i == selected){
                if(i == 5){
                    // sair
                    printf(" %s%s\n"RESET, fundoSelecaoSair, opcoes[i-1]);
                } else {
                    printf(" %s%s\n"RESET, fundoSelecaoNormal, opcoes[i-1]);
                }
            } else {
                if(i == 5){
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
        printf(GREEN"|                      ABERTO                        |   "YELLOW"|                   EM ANDAMENTO                     |   "RED"|                       FECHADO                      |\n"RESET);
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

    
    int fimLoop = (paginaAtual == 1) ? 40 - 13 : 40; // primeira pagina tem 13 linhas a menos (header)
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
            case '4':
                // data e hora
                printf("%s| %sData e Hora: %s%-37s %s|"RESET, GREEN, MAGENTA, WHITE, buf1[i1] + 1, GREEN);
                break;
            case '5':
                // descricao
                printf("%s| %s%-50s %s|"RESET, GREEN, CYAN, buf1[i1] + 1, GREEN);
                break;
            case '6':
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
            printf("%s| %s%-50s %s|"RESET, YELLOW, BLUE, buf2[i2] + 1, YELLOW);
            break;

        case '2':
            // prioridade
            printf("%s| %sPrioridade: %s%-38s %s|"RESET, YELLOW, MAGENTA, WHITE, buf2[i2] + 1, YELLOW);
            break;
        case '3':
            // criador
            printf("%s| %sCriador: %s%-41s %s|"RESET, YELLOW, MAGENTA, WHITE, buf2[i2] + 1, YELLOW);
            break;
        case '4':
            // data e hora
            printf("%s| %sData e Hora: %s%-37s %s|"RESET, YELLOW, MAGENTA, WHITE, buf2[i2] + 1, YELLOW);
            break;
        case '5':
            // descricao
            printf("%s| %s%-50s %s|"RESET, YELLOW, CYAN, buf2[i2] + 1, YELLOW);
            break;
        case '6':
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
            printf("%s| %s%-50s %s|"RESET, RED, BLUE, buf3[i3] + 1, RED);
            break;

        case '2':
            // prioridade
            printf("%s| %sPrioridade: %s%-38s %s|"RESET, RED, MAGENTA, WHITE, buf3[i3] + 1, RED);
            break;
        case '3':
            // criador
            printf("%s| %sCriador: %s%-41s %s|"RESET, RED, MAGENTA, WHITE, buf3[i3] + 1, RED);
            break;
        case '4':
            // data e hora
            printf("%s| %sData e Hora: %s%-37s %s|"RESET, RED, MAGENTA, WHITE, buf3[i3] + 1, RED);
            break;
        case '5':
            // descricao
            printf("%s| %s%-50s %s|"RESET, RED, CYAN, buf3[i3] + 1, RED);
            break;
        case '6':
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
    if(selected == 6){
        // pagina selecionada
        printf(BG_BLUE" <- Pagina %d/%d ->"RESET, paginaAtual, totalPaginas);
    } else {
        printf(BLUE" <- Pagina %d/%d ->"RESET, paginaAtual, totalPaginas);
    }
    fflush(stdout);
}

/// @brief menu para visualizar chamados
void menuVisualizarChamados(void){
    // menu tipo 4
    // opcao 1 = Atender proximo chamado
    // opcao 2 = Concluir chamado em andamento
    // opcao 3 = Deletar chamado fechado

    char* opcoes[5] = {
            "Atender proximo chamado",
            "Concluir chamado em andamento",
            "Deletar chamado fechado",
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
    esperar_tamanho_minimo(40, 170);
    if(terminalPequenoAlertado){
        
        terminalPequenoAlertado = 0;
    }
    clear();
    carregarBufferChamados(&matrizAberto, &matrizAndamento, &matrizFechado, &linhasAberto, &linhasAndamento, &linhasFechado, &divisaoLinhas, &totalPaginas);
    printarBufferChamados(divisaoLinhas, matrizAberto, matrizAndamento, matrizFechado, linhasAberto, linhasAndamento, linhasFechado, paginaAtual, totalPaginas, selected);

    while(1){
        KeyCode k = userGetKey();
        if(k == RESIZE_EVENT){
            esperar_tamanho_minimo(40, 170);
            if(terminalPequenoAlertado){
                clear();
                printarBufferChamados(divisaoLinhas, matrizAberto, matrizAndamento, matrizFechado, linhasAberto, linhasAndamento, linhasFechado, paginaAtual, totalPaginas, selected);
                terminalPequenoAlertado = 0;
            }
            continue;
        }else if(k == KC_ESC){
            popPilha(estruturasGlobais.pil);
            break;
        }
        if(paginaAtual == 1){
            char* corTexto1 = BLUE;
            char* fundoTexto2 = BG_BLUE;
           if(k == KC_DOWN){
                if(selected < 6){  // máximo 6 opções
                    // desmarcar opção atual
                    updateOption(selected + 2, opcoes[selected-1], "", (selected == 5 ? RED : BLUE));
                    selected++;
                    // marcar opção nova
                    if(selected == 6){
                        // pagina
                        char fimPaginaStr[20];
                        snprintf(fimPaginaStr, 20, " <- Pagina %d/%d ->", paginaAtual, totalPaginas);
                        updateOption(42, fimPaginaStr, BG_BLUE, "");
                    } else {
                        updateOption(selected + 2, opcoes[selected-1], (selected == 5 ? BG_RED : BG_BLUE), "");
                    }
                }
            }else if(k == KC_UP){
                if(selected > 1){
                    // desmarcar opção atual
                    if(selected == 6){
                        char fimPaginaStr[20];
                        snprintf(fimPaginaStr, 20, " <- Pagina %d/%d ->", paginaAtual, totalPaginas);
                        updateOption(42, fimPaginaStr, "", BLUE);
                    } else {
                        updateOption(selected + 2, opcoes[selected-1], "", (selected == 5 ? RED : BLUE));
                    }
                    selected--;
                    // marcar opção nova
                    updateOption(selected + 2, opcoes[selected-1], (selected == 5 ? BG_RED : BG_BLUE), "");
                }
            
            }else if(k == KC_RIGHT && selected == 6){
                if(paginaAtual < totalPaginas){
                    paginaAtual++;
                    clear();
                    printarBufferChamados(divisaoLinhas, matrizAberto, matrizAndamento, matrizFechado, linhasAberto, linhasAndamento, linhasFechado, paginaAtual, totalPaginas, selected);
                }
            }else if(k == KC_LEFT && selected == 6){
                if(paginaAtual > 1){
                    paginaAtual--;
                    clear();
                    printarBufferChamados(divisaoLinhas, matrizAberto, matrizAndamento, matrizFechado, linhasAberto, linhasAndamento, linhasFechado, paginaAtual, totalPaginas, selected);
                }
            }else if(k == KC_ENTER && selected < 6){
                if(selected == 1){
                    // atendender proximo chamado
                    if(linhasAberto == 0){
                        // nao ha chamados abertos
                        menuHandler m = createMenu(12);
                        pushPilha(estruturasGlobais.pil, m);
                        break;
                    } else {
                        menuHandler m = createMenu(13);
                        pushPilha(estruturasGlobais.pil, m);
                        break;
                    }
                }else if(selected == 2){
                    // concluir chamado em andamento
                    if(linhasAndamento == 0){
                        // nao ha chamados em andamento
                        menuHandler m = createMenu(15);
                        pushPilha(estruturasGlobais.pil, m);
                    } else {
                        menuHandler m = createMenu(14);
                        pushPilha(estruturasGlobais.pil, m);
                    }
                    break;
                }else if(selected == 3){
                    if(linhasFechado == 0){
                        // nao ha chamados fechados
                        menuHandler m = createMenu(17);
                        pushPilha(estruturasGlobais.pil, m);
                    } else {
                        // deletar chamado fechado
                        menuHandler m = createMenu(18);
                        pushPilha(estruturasGlobais.pil, m);
                    }
                    break;
                }else if(selected == 4){
                    // voltar
                    popPilha(estruturasGlobais.pil);
                    break;
                }else if(selected == 5){
                    // sair
                    menuHandler m = createMenu(9);
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

// =============================================
// MENU EDITAR USUARIOS
// =============================================

// funcao para menu usuarios
static void menuEditHandlerUsuarios(int userAdm, char* user){
    // se e adm ou nao (0 ou 1)
}


// =============================================
// MENU CONCLUIR CHAMADO ANDAMENTO
// =============================================


/// @brief carregar os dados do menu de concluir chamado em andamento
/// @param totalPaginas total de paginas
/// @param divisaoPaginas matriz de divisao de paginas
/// @param divisaoLinhas matriz de divisao de linhas
static void carregarDadosMenuConcluirChamado(int * totalPaginas, int ***divisaoPaginas, int *** divisaoLinhas){
    *totalPaginas = 1;
    fila* fAndamento = estruturasGlobais.filaAndamento;
    noFila* no = fAndamento->first;
    int count = 0;
    int linhasAtual = 8; // primeira pagina tem header;
    while(no != NULL){
        chamado cham = no->chamado;
        // descricao
        char temp[7][50];
        int linhasDesc = quebrarDescricao(cham.descricao, temp);
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
    no = fAndamento->first;
    int paginaAtual = 0;
    int chamadoAtualNaPagina = 0;
    linhasAtual = 8; // primeira pagina tem header
    (*divisaoLinhas)[0][0] = 9; // primeira linha do primeiro chamado
    while(no != NULL){
        chamado cham = no->chamado;
        // descricao
        char temp[7][50];
        int linhasDesc = quebrarDescricao(cham.descricao, temp);
        linhasAtual += linhasDesc + 7; // 7 linhas fixas + linhas da desc
        if(linhasAtual > 40){
            // nova pagina
            paginaAtual++;
            linhasAtual = linhasDesc + 7; // resetar linhasAtual para o novo chamado
            chamadoAtualNaPagina = 0;
        }
        
        // preencher divisao paginas e linhas
        (*divisaoPaginas)[paginaAtual][chamadoAtualNaPagina] = 1; // marcar que tem chamado
        (*divisaoLinhas)[paginaAtual][chamadoAtualNaPagina + 1] = linhasAtual + 1; // marcar linha inicial do chamado
        chamadoAtualNaPagina++;
        no = no->prox;
    }
    
}

/// @brief printar o menu de concluir chamado em andamento 
/// @param divisaoPaginas matriz de divisao de paginas
/// @param divisaoLinhas matriz de divisao de linhas
/// @param totalPaginas total de paginas
/// @param paginaAtual pagina atual
/// @param selected chamado ou opcao selecionada
static void printarMenuConcluirChamado(int ** divisaoPaginas, int ** divisaoLinhas, int totalPaginas, int paginaAtual, int selected){
    fila* fAndamento = estruturasGlobais.filaAndamento;
    noFila* no = fAndamento->first;
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
        chamado cham = no->chamado;
        char * color = YELLOW;
        if((selected == count + 3 && paginaAtual == 1) || (selected == count +1 && paginaAtual > 1)){
            // chamado selecionado
            color = BLUE;
        }
        printf("%s+----------------------------------------------------+\n"RESET, color);
        printf("%s| %s%-50s %s|\n"RESET, color, BLUE, cham.titulo, color);
        
        // prioridade
        printf("%s| %sPrioridade: %s%-38d %s|\n", color, MAGENTA, WHITE, cham.prioridade, color);
        
        // criador
        printf("%s| %sCriador: %s%-41s %s|\n"RESET, color, MAGENTA, WHITE, cham.criador, color);

        // data e hora
        printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
        color, MAGENTA, WHITE,
        cham.tempoComplexo.dia, cham.tempoComplexo.mes, cham.tempoComplexo.ano,
        cham.tempoComplexo.horas, cham.tempoComplexo.minutos, cham.tempoComplexo.segundos,
        "", color);
        
        // linha vazia
        printf("%s| %-50s %s|\n"RESET, color, "", color);
        // descricao
        char descricao[7][50] = {0};
        int linhas = quebrarDescricao(cham.descricao, descricao);
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
void menuHandlerConcluirChamado(void){
    esperar_tamanho_minimo(42, 60);
    if(terminalPequenoAlertado){   
        terminalPequenoAlertado = 0;
    }
    clear();
    int totalPaginas = 0;
    int ** divisaoPaginas = NULL;
    int ** divisaoLinhas = NULL;
    carregarDadosMenuConcluirChamado(&totalPaginas, &divisaoPaginas, &divisaoLinhas);
    int paginaAtual = 1;
    int selected = 1;

    printarMenuConcluirChamado(divisaoPaginas, divisaoLinhas, totalPaginas, paginaAtual, selected);
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
                printarMenuConcluirChamado(divisaoPaginas, divisaoLinhas, totalPaginas, paginaAtual, selected);
                terminalPequenoAlertado = 0;
            }
        }else if(k == KC_ESC){
            popPilha(estruturasGlobais.pil);
            break;
        }else if(k == KC_UP){
            if(selected == 2 && paginaAtual == 1){
                // desmarcar sair
                updateOption(5, "Sair", "", RED);
                // marcar voltar
                updateOption(4, "Voltar", BG_BLUE, "");
                selected--;
            }else if(selected == 3 && paginaAtual == 1){
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
                    printarMenuConcluirChamado(divisaoPaginas, divisaoLinhas, totalPaginas, paginaAtual, selected);
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
            }else if(selected == 2 && paginaAtual == 1){
                // desmarcar sair
                updateOption(5, "Sair", "", RED);
                selected++;
                updateChamadoAndamento(paginaAtual, selected - 2, divisaoLinhas, divisaoPaginas, 1);
            }else{
                if(selected == ultimaOpcao && paginaAtual < totalPaginas){
                    selected = 1;
                    paginaAtual++;
                    clear();
                    printarMenuConcluirChamado(divisaoPaginas, divisaoLinhas, totalPaginas, paginaAtual, selected);
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
                printarMenuConcluirChamado(divisaoPaginas, divisaoLinhas, totalPaginas, paginaAtual, selected);
            }
        }else if(k == KC_LEFT){
            if(paginaAtual > 1){
                paginaAtual--;
                selected = 1;
                clear();
                printarMenuConcluirChamado(divisaoPaginas, divisaoLinhas, totalPaginas, paginaAtual, selected);
            }
        }else if(k == KC_ENTER){
            if(paginaAtual == 1 && selected == 1){
                // voltar
                popPilha(estruturasGlobais.pil);
                lastSelected = 1;
                break;
            }else if(paginaAtual == 1 && selected == 2){
                // sair
                menuHandler m = createMenu(9);
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
                menuHandler m = createMenu(16);
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

// =================================================================
// Menu Deletar Chamado Fechado
// =================================================================


/// @brief carregar os dados do menu de concluir chamado em andamento
/// @param totalPaginas total de paginas
/// @param divisaoPaginas matriz de divisao de paginas
/// @param divisaoLinhas matriz de divisao de linhas
static void carregarDadosDeletarChamado(int * totalPaginas, int ***divisaoPaginas, int *** divisaoLinhas){
    *totalPaginas = 1;
    filaDupla* fd = estruturasGlobais.filadupla;
    noDuplo* no = fd->inicio;
    int count = 0;
    int linhasAtual = 9; // primeira pagina tem header;
    while(no != NULL){
        chamado cham = no->chamado;
        // descricao
        char temp[7][50];
        int linhasDesc = quebrarDescricao(cham.descricao, temp);
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
    no = fd->inicio;
    int paginaAtual = 0;
    int chamadoAtualNaPagina = 0;
    linhasAtual = 9; // primeira pagina tem header
    (*divisaoLinhas)[0][0] = 10; // primeira linha do primeiro chamado
    while(no != NULL){
        chamado cham = no->chamado;
        // descricao
        char temp[7][50];
        int linhasDesc = quebrarDescricao(cham.descricao, temp);
        linhasAtual += linhasDesc + 7; // 7 linhas fixas + linhas da desc
        if(linhasAtual > 40){
            // nova pagina
            paginaAtual++;
            linhasAtual = linhasDesc + 7; // resetar linhasAtual para o novo chamado
            chamadoAtualNaPagina = 0;
        }
        
        // preencher divisao paginas e linhas
        (*divisaoPaginas)[paginaAtual][chamadoAtualNaPagina] = 1; // marcar que tem chamado
        (*divisaoLinhas)[paginaAtual][chamadoAtualNaPagina + 1] = linhasAtual + 1; // marcar linha inicial do chamado
        chamadoAtualNaPagina++;
        no = no->prox;
    }
}

/// @brief printar o menu de deletar chamado fechado 
/// @param divisaoPaginas matriz de divisao de paginas
/// @param divisaoLinhas matriz de divisao de linhas
/// @param totalPaginas total de paginas
/// @param paginaAtual pagina atual
/// @param selected chamado ou opcao selecionada
static void printarMenuDeletarChamado(int ** divisaoPaginas, int ** divisaoLinhas, int totalPaginas, int paginaAtual, int selected){
    filaDupla* fd = estruturasGlobais.filadupla;
    noDuplo* no = fd->inicio;
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
        chamado cham = no->chamado;
        char * color = RED;
        if((selected == count + 3 && paginaAtual == 1) || (selected == count +1 && paginaAtual > 1)){
            // chamado selecionado
            color = BLUE;
        }
        printf("%s+----------------------------------------------------+\n"RESET, color);
        printf("%s| %s%-50s %s|\n"RESET, color, BLUE, cham.titulo, color);
        
        // prioridade
        printf("%s| %sPrioridade: %s%-38d %s|\n", color, MAGENTA, WHITE, cham.prioridade, color);
        
        // criador
        printf("%s| %sCriador: %s%-41s %s|\n"RESET, color, MAGENTA, WHITE, cham.criador, color);

        // data e hora
        printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
        color, MAGENTA, WHITE,
        cham.tempoComplexo.dia, cham.tempoComplexo.mes, cham.tempoComplexo.ano,
        cham.tempoComplexo.horas, cham.tempoComplexo.minutos, cham.tempoComplexo.segundos,
        "", color);
        
        // linha vazia
        printf("%s| %-50s %s|\n"RESET, color, "", color);
        // descricao
        char descricao[7][50] = {0};
        int linhas = quebrarDescricao(cham.descricao, descricao);
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
void menuHandlerDeletarChamado(void){
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

    printarMenuDeletarChamado(divisaoPaginas, divisaoLinhas, totalPaginas, paginaAtual, selected);
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
                printarMenuDeletarChamado(divisaoPaginas, divisaoLinhas, totalPaginas, paginaAtual, selected);
                terminalPequenoAlertado = 0;
            }
        }else if(k == KC_ESC){
            popPilha(estruturasGlobais.pil);
            break;
        }else if(k == KC_UP){
            if(selected == 2 && paginaAtual == 1){
                updateOption(4, "Deletar todos os chamados fechados", BG_BLUE, "");
                updateOption(5, "Voltar", "", BLUE);
                selected--;
            }else if(selected == 3 && paginaAtual == 1){
                updateOption(5, "Voltar", BG_BLUE, "");
                updateOption(6, "Sair", "", RED);
                selected--;
            }else if(selected == 4 && paginaAtual == 1){
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
                    printarMenuDeletarChamado(divisaoPaginas, divisaoLinhas, totalPaginas, paginaAtual, selected);
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
            }else if(selected == 2 && paginaAtual == 1){
                // desmarcar sair
                updateOption(5, "Voltar", "", BLUE);
                selected++;
                updateOption(6, "Sair", BG_RED, "");
            }else if(selected == 3 && paginaAtual == 1){
                // desmarcar sair
                updateOption(6, "Sair", "", RED);
                selected++;
                updateChamadoFechado(paginaAtual, selected - 3, divisaoLinhas, divisaoPaginas, 1);
            }else{
                if(selected == ultimaOpcao && paginaAtual < totalPaginas){
                    selected = 1;
                    paginaAtual++;
                    clear();
                    printarMenuDeletarChamado(divisaoPaginas, divisaoLinhas, totalPaginas, paginaAtual, selected);
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
                printarMenuDeletarChamado(divisaoPaginas, divisaoLinhas, totalPaginas, paginaAtual, selected);
            }
        }else if(k == KC_LEFT){
            if(paginaAtual > 1){
                paginaAtual--;
                selected = 1;
                clear();
                printarMenuDeletarChamado(divisaoPaginas, divisaoLinhas, totalPaginas, paginaAtual, selected);
            }
        }else if(k == KC_ENTER){
            if(paginaAtual == 1 && selected == 1){
                // deletar todos chamados fechados
                menuHandler m = createMenu(20);
                pushPilha(estruturasGlobais.pil, m);
                lastSelected = 1;
                break;
            }else if(paginaAtual == 1 && selected == 2){
                // voltar
                popPilha(estruturasGlobais.pil);
                lastSelected = 2;
                break;
                // apenas por seguranca (isso nao vai ser usado)
                break;
            }else if(selected == 3 && paginaAtual == 1){
                // sair
                menuHandler m = createMenu(9);
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
                menuHandler m = createMenu(19);
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