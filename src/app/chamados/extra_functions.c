#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "all.h"

/// @brief função para o menu de atender o próximo chamado  
void menuAtenderProximoChamado(void){
    filaPrioridade* fp = estruturasGlobais.chamadosAbertosComPrioridade;
    fila* f = estruturasGlobais.chamadosAbertosSemPrioridade;
    noFila* no = f->first;
    chamado* chamadoAtender;
    if(fp->n > 0){
        chamadoAtender = (chamado*)fp->elementos[0];
    }else{
        chamadoAtender = (chamado*)no->dado;
    }

    printf(GREEN"+----------------------------------------------------+"RESET);
    printf("\n");
    // titulo
    printf("%s| %s%-50s %s|\n"RESET, GREEN, BLUE, (*chamadoAtender).titulo, GREEN);
    
    // prioridade
    char bufPrioridade[15] = "Maxima";
    switch(chamadoAtender->prioridade){
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
    printf("%s| %sPrioridade: %s%-38s %s|\n", GREEN, MAGENTA, WHITE, bufPrioridade, GREEN);
    
    // criador
    printf("%s| %sCriador: %s%-41s %s|\n"RESET, GREEN, MAGENTA, WHITE, (*chamadoAtender).criador, GREEN);
    
    // data e hora
    printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
       GREEN, MAGENTA, WHITE,
       (*chamadoAtender).tempoComplexo.dia, (*chamadoAtender).tempoComplexo.mes, (*chamadoAtender).tempoComplexo.ano,
       (*chamadoAtender).tempoComplexo.horas, (*chamadoAtender).tempoComplexo.minutos, (*chamadoAtender).tempoComplexo.segundos,
       "", GREEN);
     
    // materiais
    fila * fMat = chamadoAtender->materiais;
    noFila * noMat = fMat->first;
    for(int i = 0; i < chamadoAtender->quantMateriais; i++){
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "%s (%d)", ((Item*)noMat->dado)->nome, chamadoAtender->quantMateriaisPorItem[i]);
        printf("%s| %s%-50s %s|\n"RESET, GREEN, WHITE, buffer, GREEN);
        noMat = noMat->prox;
    }

    //linha vazia
    printf(GREEN"| %-50s |\n"RESET, "");

    // descricao
    char descricao[7][50] = {0};
    int linhas = quebrarDescricao((*chamadoAtender).descricao, descricao);
    for(int i = 0; i < linhas; i++){
        printf("%s| %s%-50s %s|\n"RESET, GREEN, CYAN, descricao[i], GREEN);
    }

    // separador
    printf(GREEN"+----------------------------------------------------+"RESET);
}

/// @brief menu para confirmar conclusao de chamado em andamento
/// @param indiceChamado indice do chamado na fila de andamento
void menuConcluirChamadoConfirmar(int indiceChamado){
    User* u = arv_buscar(estruturasGlobais.atendentes, usuario);
    fila* f = u->filaChamados;
    noFila* no = f->first;
    for(int i = 0; i < indiceChamado; i++){
        no = no->prox;
    }
    chamado* cham = (chamado*)no->dado;
    printf(YELLOW"+----------------------------------------------------+"RESET);
    printf("\n");
    // titulo
    printf("%s| %s%-50s %s|\n"RESET, YELLOW, BLUE, (*cham).titulo, YELLOW);
    
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
    printf("%s| %sPrioridade: %s%-38s %s|\n", YELLOW, MAGENTA, WHITE, bufPrioridade, YELLOW);
    
    // criador
    printf("%s| %sCriador: %s%-41s %s|\n"RESET, YELLOW, MAGENTA, WHITE, (*cham).criador, YELLOW);
    
    // data e hora
    printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
       YELLOW, MAGENTA, WHITE,
       (*cham).tempoComplexo.dia, (*cham).tempoComplexo.mes, (*cham).tempoComplexo.ano,
       (*cham).tempoComplexo.horas, (*cham).tempoComplexo.minutos, (*cham).tempoComplexo.segundos,
       "", YELLOW);

    // materiais
    fila * fMat = cham->materiais;
    noFila * noMat = fMat->first;
    for(int i = 0; i < cham->quantMateriais; i++){
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "%s (%d)", ((Item*)noMat->dado)->nome, cham->quantMateriaisPorItem[i]);
        printf("%s| %s%-50s %s|\n"RESET, YELLOW, WHITE, buffer, YELLOW);
        noMat = noMat->prox;
    }

    //linha vazia
    printf(YELLOW"| %-50s |\n"RESET, "");

    // descricao
    char descricao[7][50] = {0};
    int linhas = quebrarDescricao((*cham).descricao, descricao);
    for(int i = 0; i < linhas; i++){
        printf("%s| %s%-50s %s|\n"RESET, YELLOW, CYAN, descricao[i], YELLOW);
    }
   
    // separador
    printf(YELLOW"+----------------------------------------------------+"RESET);
    
}

/// @brief menu para confirmar exclusao de chamado fechado
/// @param indiceChamado indice do chamado na fila dupla
void menuDeletarChamadoConfirmar(int indiceChamado){
    fila* fFechados = estruturasGlobais.chamadosConcluidos;
    noFila* no = fFechados->first;
    for(int i = 0; i < indiceChamado; i++){
        no = no->prox;
    }
    chamado* cham = (chamado*)no->dado;
    printf(RED"+----------------------------------------------------+"RESET);
    printf("\n");
    // titulo
    printf("%s| %s%-50s %s|\n"RESET, RED, BLUE, (*cham).titulo, RED);
    
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
    printf("%s| %sPrioridade: %s%-38s %s|\n", RED, MAGENTA, WHITE, bufPrioridade, RED);
    
    // criador
    printf("%s| %sCriador: %s%-41s %s|\n"RESET, RED, MAGENTA, WHITE, cham->criador, RED);
    
    // atendente
    char * bufAtendente;
    int len = strlen(cham->atendente);
    if(len == 0){
        bufAtendente = "Nao foi atendido";
    }else{
        bufAtendente = cham->atendente;
    }
    printf("%s| %sAtendente: %s%-39s %s|\n"RESET, RED, MAGENTA, WHITE, bufAtendente, RED);

    // data e hora
    printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
       RED, MAGENTA, WHITE,
       cham->tempoComplexo.dia, cham->tempoComplexo.mes, cham->tempoComplexo.ano,
       cham->tempoComplexo.horas, cham->tempoComplexo.minutos, cham->tempoComplexo.segundos,
       "", RED);

    
    // materiais
    fila * f = cham->materiais;
    noFila * noMat = f->first;
    for(int i = 0; i < cham->quantMateriais; i++){
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "%s (%d)", ((Item*)noMat->dado)->nome, cham->quantMateriaisPorItem[i]);
        printf("%s| %s%-50s %s|\n"RESET, RED, WHITE, buffer, RED);
        noMat = noMat->prox;
    }

    //linha vazia
    printf(RED"| %-50s |\n"RESET, "");

    // descricao
    char descricao[7][50] = {0};
    int linhas = quebrarDescricao(cham->descricao, descricao);
    for(int i = 0; i < linhas; i++){
        printf("%s| %s%-50s %s|\n"RESET, RED, CYAN, descricao[i], RED);
    }
   
    // separador
    printf(RED"+----------------------------------------------------+"RESET);
}

/// @brief conteudo extra para o menu de confirmacao de cancelar chamado
/// @param indiceChamado indice do chamado na fila
/// @param tipo 1-aberto, 2-andamento, 3-suspenso
void menuCancelarChamadoConfirmar(int indiceChamado, int tipo){
    chamado* cham= NULL;
    char* color = RESET;
    if(tipo == 1){
        color = GREEN;
        fila * f = estruturasGlobais.chamadosAbertosSemPrioridade;
        filaPrioridade* fp = estruturasGlobais.chamadosAbertosComPrioridade;
        noFila* no = f->first;

        int contador = 0;
        if(indiceChamado < fp->n){
            // chamado com prioridade
            cham = (chamado*)fp->elementos[indiceChamado];
        }else{
            // chamado sem prioridade
            contador = fp->n;
            while(no != NULL){
                if(contador == indiceChamado){
                    cham = (chamado*)no->dado;
                    break;
                }
                contador++;
                no = no->prox;
            }
        }
    }else if(tipo == 2){
        color = YELLOW;
        filaDupla* f = estruturasGlobais.chamadosAndamento;
        noDuplo* no = f->inicio;
        for(int i = 0; i < indiceChamado; i++){
            no = no->prox;
        }
        cham = (chamado*)no->dado;
    }else{
        color = RED;
        filaDupla* f = estruturasGlobais.chamadosSuspensos;
        noDuplo* no = f->inicio;
        for(int i = 0; i < indiceChamado; i++){
            no = no->prox;
        }
        cham = (chamado*)no->dado;
    }
    if(cham == NULL) return;
    printf("%s+----------------------------------------------------+"RESET, color);
    printf("\n");
    // titulo
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
    
    // atendente
    if(tipo == 2){
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

    //linha vazia
    printf("%s| %-50s |\n"RESET, color, "");

    // descricao
    char descricao[7][50] = {0};
    int linhas = quebrarDescricao((*cham).descricao, descricao);
    for(int i = 0; i < linhas; i++){
        printf("%s| %s%-50s %s|\n"RESET, color, CYAN, descricao[i], color);
    }
   
    // separador
    printf("%s+----------------------------------------------------+"RESET, color);
}

void criadorMenu(void){
    printf("HelpDesk\n");
    printf("Desenvolvido por:\n");
    printf(BLUE"Gabriel Mazui Azevedo\n");
    printf("Obrigado por usar meu programa\n");
    printf(
    BLUE
    "                    *****************\n"
    "               ******               ******\n"
    "           ****                           ****\n"
    "        ****                                 ***\n"
    "      ***                                       ***\n"
    "     **           ***               ***           **\n"
    "   **           *******           *******          ***\n"
    "  **            *******           *******            **\n"
    " **             *******           *******             **\n"
    " **               ***               ***               **\n"
    "**                                                     **\n"
    "**       *                                     *       **\n"
    "**      **                                     **      **\n"
    " **   ****                                     ****   **\n"
    " **      **                                   **      **\n"
    "  **       ***                             ***       **\n"
    "   ***       ****                       ****       ***\n"
    "     **         ******             ******         **\n"
    "      ***            ***************            ***\n"
    "        ****                                 ****\n"
    "           ****                           ****\n"
    "               ******               ******\n"
    "                    *****************\n"
    "\033[0m"
    );
}
