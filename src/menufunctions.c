#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "menu.h"
#include "pilha.h"
#include "fila.h"
#include "filaprioridade.h"
#include "utils.h"
#include "menufunctions.h"
#include "colors.h"
#include "filadupla.h"

#define TAM_LINHA 52

// funcao para criar um struct com os dados do tempo de agora
static Tempo tempoAgora(void){
    Tempo t;

    time_t agora = time(NULL);           // pega o tempo atual em segundos
    struct tm *info = localtime(&agora); // converte para data/hora local

    // copia para a struct personalizada
    t.segundos = info->tm_sec;
    t.minutos = info->tm_min;
    t.horas   = info->tm_hour;
    t.dia    = info->tm_mday;
    t.mes    = info->tm_mon + 1;     // +1 porque tm_mon vai de 0 a 11
    t.ano    = info->tm_year + 1900; // +1900 porque tm_year conta desde 1900
    return t;
}

// funcao para o menu de criar chamado (preferencia e tiulo do chamado)
static void menuCriarChamadoHandler(int prioridadeParametro, int comPreferencia, Estruturas dados, char* user){
    // menu tipo 3

    // 1 = Sim
    // 2 = nao

    // caso preferencia for maior que 0, significa que ja foi escolhido
    // entao pode ir para o proximo passo

    char tituloChamada[50]; // buffer para o titulo da chamada
    int prioridade = prioridadeParametro; // padrao 0 
    
    char* pergunta = "Qual a prioridade desse chamado?";
    int entradaErrada = 0;
    // descobre a prioridade , caso tiver
    if(comPreferencia == 1 && prioridade == 0){
        while(1){
            printf("\n");
            repetirChar(15, '-', BLUE);
            printf(BLUE"%s"RESET, "Criar chamado");
            repetirChar(15, '-', BLUE);
            printf("\n");
            if(entradaErrada){
                printf(RED" Escolha um numero maior que 0"RESET);
                printf("\n");
            }
            printf(GREEN" %s"RESET, pergunta);
            printf("\n\n ");
            fflush(stdout);
            
            char digitos[7];
            int escVerification = 0;
            // le o input de numero
            inputNumeroASCII(digitos, sizeof(digitos) - 1, WHITE, &escVerification);
            
            // se tiver esc volta pro menu de pergunta se tem prioridade
            if(escVerification){
                popPilha(dados.pil);
                clear();
                return;
            }
            // transforma o vetor de char de digitos em numero    
            for (int i = 0; digitos[i] != '\0'; i++) {
                char c = digitos[i];
                if (c >= '0' && c <= '9') {
                    prioridade = prioridade * 10 + (c - '0'); // constrói o número
                }
            }
            if(prioridade <= 0){
                // continua o loop
                clear();
                entradaErrada = 1;
            }else{
                clear();
                break;
            }
            clear();
        }
    }


    char* pergunta2 = "Qual o titulo do chamado?";
    // parte 2
    // pergunta o titulo da chamada
    printf("\n");
    repetirChar(15, '-', BLUE);
    printf(BLUE"%s"RESET, "Criar chamado");
    repetirChar(15, '-', BLUE);
    printf("\n");
    
    printf(GREEN" %s"RESET, pergunta2);
    printf("\n\n ");
    fflush(stdout);
    
    int escVerification = 0; // variavel para verificar se o ESC foi indentificado no inputASCII
    inputASCII(tituloChamada, 40, WHITE, &escVerification); // ler input
    // funcao para verificar se e uma string valida
    if(escVerification){
        // se for pressionado o esc retorna para o primeiro menu
        clear();
        if(comPreferencia){
            // preferencia 0 para permitir que seja perguntado de novo
            menuCriarChamadoHandler(0, comPreferencia, dados, user);
        }else{
            popPilha(dados.pil);
            // caso nao tiver preferencia, volta para o menu de criar chamado
        }
        // chamo a funcao novamente, para voltar ao menu de qual a preferencia
        // caso apertar o esc denovo, ele voltar para o menu de criar chamado original
        return;
    }
    clear();
    
    // descrever o problema
    char* pergunta3 = "Descreva melhor o seu problema:";
    printf("\n");
    repetirChar(15, '-', BLUE);
    printf(BLUE"%s"RESET, "Criar chamado");
    repetirChar(15, '-', BLUE);
    printf("\n");
    
    printf(GREEN" %s"RESET, pergunta3);
    printf("\n\n ");
    fflush(stdout);
    char descricaoChamada[350]; // buffer para a descricao da chamada
    escVerification = 0; // variavel para verificar se o ESC foi indentificado no inputASCII
    inputASCII(descricaoChamada, 300, WHITE, &escVerification); // ler input
    // funcao para verificar se e uma string valida

    if(escVerification){
        clear();
        menuCriarChamadoHandler(prioridade, comPreferencia, dados, user);
        return;
    }
    clear();


    chamado cham;
    cham.status = 1;
    
    strncpy(cham.titulo, tituloChamada, sizeof(cham.titulo)-1);
    cham.titulo[sizeof(cham.titulo)-1] = '\0'; // garante terminação

    strncpy(cham.descricao, descricaoChamada, sizeof(cham.descricao)-1);
    cham.descricao[sizeof(cham.descricao)-1] = '\0';

    strncpy(cham.criador, user, sizeof(cham.criador)-1);
    cham.criador[sizeof(cham.criador)-1] = '\0';

    Tempo t = tempoAgora();
    time_t agora = time(NULL);          // pega o tempo atual em segundos
    cham.tempoSimples = agora;
    cham.tempoComplexo = t;
    
    if(comPreferencia == 1){
        popPilha(dados.pil);
        popPilha(dados.pil);
        // realizo o pop duas vezes, para voltar direto para o menu principal
        // adiciona na fila de prioridade

        //adiciona na fila de prioridade
        filaPrioridade* f = dados.filaPrioridade;
        cham.prioridade = prioridade;
        filaPrioridadeInserir(f, cham);
    }else{
        fila* f = dados.filaNormal;
        cham.prioridade = 0;
        filaInserir(f, cham);
        popPilha(dados.pil);
        popPilha(dados.pil);
        // realizo o pop duas vezes, para voltar direto para o menu principal
        // adiciona na fila normal
        // tira o esse menu da pilha
    }
}

// visualizar chamados
static void menuEditHandlerChamados(int opcao, Estruturas dados, char* user){
    // menu tipo 5
    // opcao 1 = Atender proximo chamado
    // opcao 2 = Concluir chamado em andamento
    // opcao 3 = Deletar chamado fechado

}

// funcao para menu usuarios
static void menuEditHandlerUsuarios(int userAdm, Estruturas dados, char* user){
    // se e adm ou nao (0 ou 1)
}

static void funcaoMainExtra(int type, int opcao, Estruturas dados, char* user){
    switch(type){
        case 3:
            // criar chamado
            menuCriarChamadoHandler(0, opcao, dados, user);
            break;
        case 5:
            // editar chamados
            break;
        case 7:
            // editar usuarios
            break;
    }
}


// --------------------------------
// FUNCOES DE CONTEUDO EXTRA
// --------------------------------

// funcao para formatar testo da descricao em varias linhas
static void quebrarDescricao(const char *texto, char destino[7][50]) {
    int linha = 0;
    const char *p = texto;

    while (*p && linha < 7) {
        int col = 0;
        int lastSpace = -1;

        while (*p && *p != '\n' && col < 49) {
            destino[linha][col] = *p;

            if (*p == ' ')
                lastSpace = col; // marca último espaço antes de encher a linha

            p++;
            col++;
        }

        
        if (*p && *p != ' ' && col >= 49 && lastSpace != -1) {
            // volta o ponteiro até o último espaço
            int retroceder = col - lastSpace - 1;
            p -= retroceder; // volta no texto original
            col = lastSpace; // corta a linha ali
        }

        // finaliza a linha
        destino[linha][col] = '\0';
        linha++;

        // pular o \n se tiver
        if (*p == '\n') p++;

        // pula espaço inicial da próxima linha, se houver
        if (*p == ' ') p++;
    }

    // Garante que as linhas restantes fiquem vazias
    for (; linha < 7; linha++)
        destino[linha][0] = '\0';
}


// funcao auxiliar para mostrar os chamados em baixo do menu de visualizar chamados
static void mostrarChamados(Estruturas dados) {
    fila *f = dados.filaNormal;
    filaPrioridade *fp = dados.filaPrioridade;
    filaDupla *fd = dados.filadupla;
    fila *fAndamento = dados.filaAndamento;

    noFila *atualFilaAberto = f ? f->first : NULL;
    noDuplo *atualFilaFechado = fd ? fd->inicio : NULL;
    noFila *atualFilaAndamento = fAndamento ? fAndamento->first : NULL;

    int fQuant = f ? f->n : 0;
    int fdQuant = fd ? fd->n : 0;
    int fAndamentoQuant = fAndamento ? fAndamento->n : 0;
    int fpQuant = fp ? fp->n : 0;

    int fUsed = 0, fdUsed = 0, fAndamentoUsed = 0, fpUsed = 0;
    printf(GREEN" +--------------------------------------------------+     "YELLOW"+--------------------------------------------------+     "RED"+--------------------------------------------------+\n"RESET);
    printf(GREEN" |                     ABERTO                       |     "YELLOW"|                   EM ANDAMENTO                   |     "RED"|                      FECHADO                     |\n"RESET);
    printf(GREEN" +--------------------------------------------------+     "YELLOW"+--------------------------------------------------+     "RED"+--------------------------------------------------+\n"RESET);


    // calcular número de linhas
    int linhasAberto = (fQuant + fpQuant) * 14;
    int linhasAndamento = fAndamentoQuant * 14;
    int linhasFechado = fdQuant * 14;

    // inicializar ponteiros como NULL
    char **matrizAberto = NULL;
    char **matrizAndamento = NULL;
    char **matrizFechado = NULL;

    // alocar matrizAberto se tiver linhas
    if (linhasAberto > 0) {
        matrizAberto = calloc(linhasAberto, sizeof(char*));
        if (!matrizAberto) { // logs
            perror("calloc matrizAberto"); exit(1); 
        }
        for (int i = 0; i < linhasAberto; i++) {
            matrizAberto[i] = calloc(52, sizeof(char)); // 51 + '\0'
            if (!matrizAberto[i]) { 
                // logs
                perror("calloc matrizAberto[i]"); exit(1); 
            }
        }
    }

    // alocar matrizAndamento se tiver linhas
    if (linhasAndamento > 0) {
        matrizAndamento = calloc(linhasAndamento, sizeof(char*));
        if (!matrizAndamento) { 
            // logs
            perror("calloc matrizAndamento"); exit(1); 
        }
        for (int i = 0; i < linhasAndamento; i++) {
            matrizAndamento[i] = calloc(52, sizeof(char));
            if (!matrizAndamento[i]) { 
                // logs
                perror("calloc matrizAndamento[i]"); exit(1); 
            }
        }
    }

    // alocar matrizFechado se tiver linhas
    if (linhasFechado > 0) {
        matrizFechado = calloc(linhasFechado, sizeof(char*));
        if (!matrizFechado) { perror("calloc matrizFechado"); exit(1); }
        for (int i = 0; i < linhasFechado; i++) {
            matrizFechado[i] = calloc(52, sizeof(char));
            if (!matrizFechado[i]) { perror("calloc matrizFechado[i]"); exit(1); }
        }
    }


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
        if(linhasAberto > 0){
            matrizAberto[linhaAtualAberto][0] = '\0';
        }
        if(linhasAndamento > 0){
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }
        if(linhasFechado > 0){
            matrizFechado[linhaAtualFechado][0] = '\0';
        }

        if (fpUsed < fpQuant) {
            matrizAberto[linhaAtualAberto][0] = '1'; // marca como titulo
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s", fp->elementos[fpUsed].titulo);
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

        if (fdUsed < fdQuant && atualFilaFechado) {
            matrizFechado[linhaAtualFechado][0] = '1'; // marca como titulo
            snprintf(matrizFechado[linhaAtualFechado] + 1, TAM_LINHA - 1, "%s", atualFilaFechado->chamado.titulo);
            linhaAtualFechado++;
        }

        // ========================================================
        // Linha 2: PRIORIDADE
        // ========================================================
        if(linhasAberto > 0){
            matrizAberto[linhaAtualAberto][0] = '\0';
        }
        if(linhasAndamento > 0){
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }
        if(linhasFechado > 0){
            matrizFechado[linhaAtualFechado][0] = '\0';
        }

        if (fpUsed < fpQuant) {
            matrizAberto[linhaAtualAberto][0] = '2'; // marca como prioridade
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%d", fp->elementos[fpUsed].prioridade);
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
        if(linhasAberto > 0){
            matrizAberto[linhaAtualAberto][0] = '\0';
        }
        if(linhasAndamento > 0){
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }
        if(linhasFechado > 0){
            matrizFechado[linhaAtualFechado][0] = '\0';
        }

        if (fpUsed < fpQuant) {
            matrizAberto[linhaAtualAberto][0] = '3'; // marca como criador
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%s", fp->elementos[fpUsed].criador);
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
        if(linhasAberto > 0){
            matrizAberto[linhaAtualAberto][0] = '\0';
        }
        if(linhasAndamento > 0){
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }
        if(linhasFechado > 0){
            matrizFechado[linhaAtualFechado][0] = '\0';
        }

        if (fpUsed < fpQuant){
            matrizAberto[linhaAtualAberto][0] = '4'; // marca como data e hora
            snprintf(matrizAberto[linhaAtualAberto] + 1, TAM_LINHA - 1, "%02d/%02d/%04d %02d:%02d:%02d",
                    fp->elementos[fpUsed].tempoComplexo.dia,
                    fp->elementos[fpUsed].tempoComplexo.mes,
                    fp->elementos[fpUsed].tempoComplexo.ano,
                    fp->elementos[fpUsed].tempoComplexo.horas,
                    fp->elementos[fpUsed].tempoComplexo.minutos,
                    fp->elementos[fpUsed].tempoComplexo.segundos);
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
        if(linhasAberto > 0){
            matrizAberto[linhaAtualAberto][0] = '\0';
        }
        if(linhasAndamento > 0){
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }
        if(linhasFechado > 0){
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

        if(linhasAberto > 0){
            matrizAberto[linhaAtualAberto][0] = '\0';
        }
        if(linhasAndamento > 0){
            matrizAndamento[linhaAtualAndamento][0] = '\0';
        }
        if(linhasFechado > 0){
            matrizFechado[linhaAtualFechado][0] = '\0';
        }

        char desc1[7][50] = {0};
        char desc2[7][50] = {0};
        char desc3[7][50] = {0};

        // fila de prioridade
        if (fpUsed < fpQuant) {
            // formatar string descricao (para nao ficar palavras cortadas na hora de printar)
            quebrarDescricao(fp->elementos[fpUsed].descricao, desc1);
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
        // fim do loop while para guardar as informacoes nas matrizes
    }

    fflush(stdout);
    // ========================================================
    // imprimir as matrizes lado a lado
    // ========================================================
    int maxLinhas = linhaAtualAberto;
    if (linhaAtualAndamento > maxLinhas) maxLinhas = linhaAtualAndamento;
    if (linhaAtualFechado > maxLinhas) maxLinhas = linhaAtualFechado;

    int indiceAtual = 0;
    while (maxLinhas > 0) {
        // imprimir linha
        char firstCharAberto = ((indiceAtual < linhasAberto) ? matrizAberto[indiceAtual][0] : ' ');
        char firstCharAndamento = ((indiceAtual < linhasAndamento) ? matrizAndamento[indiceAtual][0] : ' ');
        char firstCharFechado = ((indiceAtual < linhasFechado) ? matrizFechado[indiceAtual][0] : ' ');

        // ---------------------------------------------------------
        switch (firstCharAberto)
        {
        case '0':
            // linha vazia
            printf(GREEN"| %-50s |"RESET, "");
            break;
        case '1':
            // titulo
            printf("%s| %s%-50s %s|"RESET, GREEN, BLUE, matrizAberto[indiceAtual] + 1, GREEN);
            break;

        case '2':
            // prioridade
            printf("%s| %sPrioridade: %s%-38s %s|"RESET, GREEN, MAGENTA, WHITE, matrizAberto[indiceAtual] + 1, GREEN);
            break;
        case '3':
            // criador
            printf("%s| %sCriador: %s%-41s %s|"RESET, GREEN, MAGENTA, WHITE, matrizAberto[indiceAtual] + 1, GREEN);
            break;
        case '4':
            // data e hora
            printf("%s| %sData e Hora: %s%-37s %s|"RESET, GREEN, MAGENTA, WHITE, matrizAberto[indiceAtual] + 1, GREEN);
            break;
        case '5':
            // descricao
            printf("%s| %s%-50s %s|"RESET, GREEN, CYAN, matrizAberto[indiceAtual] + 1, GREEN);
            break;
        case '6':
            // separador
            printf(GREEN" +--------------------------------------------------+"RESET);
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
            printf("%s| %s%-50s %s|"RESET, YELLOW, BLUE, matrizAndamento[indiceAtual] + 1, YELLOW);
            break;

        case '2':
            // prioridade
            printf("%s| %sPrioridade: %s%-38s %s|"RESET, YELLOW, MAGENTA, WHITE, matrizAndamento[indiceAtual] + 1, YELLOW);
            break;
        case '3':
            // criador
            printf("%s| %sCriador: %s%-41s %s|"RESET, YELLOW, MAGENTA, WHITE, matrizAndamento[indiceAtual] + 1, YELLOW);
            break;
        case '4':
            // data e hora
            printf("%s| %sData e Hora: %s%-37s %s|"RESET, YELLOW, MAGENTA, WHITE, matrizAndamento[indiceAtual] + 1, YELLOW);
            break;
        case '5':
            // descricao
            printf("%s| %s%-50s %s|"RESET, YELLOW, CYAN, matrizAndamento[indiceAtual] + 1, YELLOW);
            break;
        case '6':
            // separador
            printf(YELLOW" +--------------------------------------------------+"RESET);
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
            printf("%s| %s%-50s %s|"RESET, RED, BLUE, matrizFechado[indiceAtual] + 1, RED);
            break;

        case '2':
            // prioridade
            printf("%s| %sPrioridade: %s%-38s %s|"RESET, RED, MAGENTA, WHITE, matrizFechado[indiceAtual] + 1, RED);
            break;
        case '3':
            // criador
            printf("%s| %sCriador: %s%-41s %s|"RESET, RED, MAGENTA, WHITE, matrizFechado[indiceAtual] + 1, RED);
            break;
        case '4':
            // data e hora
            printf("%s| %sData e Hora: %s%-37s %s|"RESET, RED, MAGENTA, WHITE, matrizFechado[indiceAtual] + 1, RED);
            break;
        case '5':
            // descricao
            printf("%s| %s%-50s %s|"RESET, RED, CYAN, matrizFechado[indiceAtual] + 1, RED);
            break;
        case '6':
            // separador
            printf(RED" +--------------------------------------------------+"RESET);
            break;
        default:
            // linha vazia
            printf("%-54s", "");
            break;
        }
        printf("\n");
        fflush(stdout);
        indiceAtual++;
        maxLinhas--;
    }

    // liberar memoria
    if(linhasAberto > 0){
        for (int i = 0; i < (fQuant+fpQuant) * 14; i++) {
            free(matrizAberto[i]);
        }
        free(matrizAberto);
    }
    if(linhasAndamento > 0){
        for (int i = 0; i < fAndamentoQuant * 14; i++) {
            free(matrizAndamento[i]);
        }
        free(matrizAndamento);
    }
    if(linhasFechado > 0){
        for (int i = 0; i < fdQuant * 14; i++) {
            free(matrizFechado[i]);
        }
        free(matrizFechado);
    }
}



// para estaticas, relatorios e logs, (tipos 6, 8, 9)
// para o tipo 4 tambem (visualizar chamados)
// as info seram printadas em baixo
static void conteudoExtra(int type, Estruturas dados){
    switch(type){
        case 4:
            // visualizar chamados
            mostrarChamados(dados);
            break;
        case 6:
            // estatisticas
            break;
        case 8:
            // relatorios
            break;
        case 9:
            // logs
            break;
    }
}


// vai criar um struct do determinado tipo de menu, e vai retornar ele
static menuHandler createMenu(int type){
    // menu 1 sempre estara na pilha, por ser a "raiz" e ter sido criada no menu.c
    // entao nao e necessario que seja criado aqui
    menuHandler menuReturn;
    switch(type){
        case 2:
            // criar chamado tem preferencia?
            menuReturn.type = 2;
            menuReturn.titulo = "Criar chamado";
            menuReturn.frase = "Esse chamado tem prioridade?";
            menuReturn.quant = 4;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";
            menuReturn.options[2] = "Voltar";
            menuReturn.options[3] = "Sair";
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;
    
        case 3:
            // funcao separada, escolher prioridade > 0 se tiver, e o nome do chamado
            menuReturn = (menuHandler){0};
            menuReturn.type = 3;
            menuReturn.titulo = "";
            menuReturn.frase = "";
            menuReturn.funcaoMain = funcaoMainExtra;
            break;
    
        case 4:
            // filtrar chamados 
            menuReturn.type = 4;
            menuReturn.titulo = "Visualizar Chamados";
            menuReturn.frase = "";
            menuReturn.quant = 5;
            menuReturn.options[0] = "Atender proximo chamado";
            menuReturn.options[1] = "Concluir chamado em andamento";
            menuReturn.options[2] = "Deletar chamado fechado";
            menuReturn.options[3] = "Voltar";
            menuReturn.options[4] = "Sair";
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = conteudoExtra;
            break;
    
        case 5:
            // funcao separada, chamados irao aparecer na direita
            // sera possivel editar pelo botao (ou seta para direita e esquerda)
            menuReturn = (menuHandler){0};
            menuReturn.type = 5;
            menuReturn.titulo = "";
            menuReturn.frase = "";
            menuReturn.funcaoMain = funcaoMainExtra;
            break;
    
        case 6:
            // Estatisticas, vai aparecer as info em baixo do menu de opcoes
            menuReturn.type = 6;
            menuReturn.titulo = "Estatisticas";
            menuReturn.frase = "";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Voltar";
            menuReturn.options[1] = "Sair";
            menuReturn.funcaoExtra = conteudoExtra;
            menuReturn.funcaoMain = NULL;
            break;
    
        case 7:
            // usuarios (caso for adm sera possivel editar)
            // infos vao ficar na direita (edicao via botao ou via seta para direita ou esquerda);
            // funcao separada
            menuReturn = (menuHandler){0};
            menuReturn.type = 7;
            menuReturn.titulo = "";
            menuReturn.frase = "";
            menuReturn.funcaoMain = funcaoMainExtra;
            break;
    
        case 8:
            // relatorios
            // infos em baixo 
            menuReturn.type = 8;
            menuReturn.titulo = "Relatorio das ultimas acoes";
            menuReturn.frase = "";
            menuReturn.quant = 3;
            menuReturn.options[0] = "Deletar todo historico de acoes";
            menuReturn.options[1] = "Voltar";
            menuReturn.options[2] = "Sair";
            menuReturn.funcaoExtra = conteudoExtra;
            menuReturn.funcaoMain = NULL;
            break;
    
        case 9:
            // logs do sistema
            // infos em baixo
            menuReturn.type = 9;
            menuReturn.titulo = "Logs do sistema";
            menuReturn.frase = "";
            menuReturn.quant = 3;
            menuReturn.options[0] = "Deletar todo historico de logs";
            menuReturn.options[1] = "Voltar";
            menuReturn.options[2] = "Sair";
            menuReturn.funcaoExtra = conteudoExtra;
            menuReturn.funcaoMain = NULL;
            break;
    
        case 10:
            // menu de Sair
            // nao tem a opcao sair nem de voltar
            menuReturn.type = 10;
            menuReturn.titulo = "Sair";
            menuReturn.frase = "Voce deseja mesmo fechar o programa?";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;
        case 11:
            // menu de deletar todos relatorios
            // nao tem a opcao sair nem de voltar
            menuReturn.type = 11;
            menuReturn.titulo = "Deletar todos relatorios";
            menuReturn.frase = "Voce deseja mesmo deletar todos os relatorios?";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;
        case 12:
            // menu de deletar logs
            // nao tem a opcao sair nem de voltar
            menuReturn.type = 12;
            menuReturn.titulo = "Deletar logs";
            menuReturn.frase = "Voce deseja mesmo deletar todos os logs?";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;
    }
    return menuReturn;
}

// funcao para esc
void menuHandlerEsc(int type, pilha* p){
    if(type == 1){
        menuHandler mSair = createMenu(10);
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
                    m = createMenu(6);
                    pushPilha(*p, m);
                    break;
                case 4:
                    // usuarios
                    m = createMenu(7);
                    pushPilha(*p, m);
                    break;
                case 5:
                    // relatorios
                    m = createMenu(8);
                    pushPilha(*p, m);
                    break;
                case 6:
                    // logs
                    m = createMenu(9);
                    pushPilha(*p, m);
                    break;
                case 7:
                    // sair
                    m = createMenu(10);
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
        case 4:
            // visualizar as acoes
            switch(selected){
                case 1:
                case 2:
                case 3:
                    // qual tipo de edicao deseja fazer
                    pushPilha(*p, createMenu(5));
                    break;
                case 4:
                    // voltar
                    popPilha(*p);
                    break;
                case 5:
                    // sair
                    pushPilha(*p, createMenu(10));
                    break;
            }
            break;
        case 6:
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
        case 8:
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
        case 9:
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
        case 10:
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
        case 11:
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
        case 12:
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
    }
}