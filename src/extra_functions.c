#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "menu.h"
#include "pilha.h"
#include "fila.h"
#include "filaprioridade.h"
#include "utils.h"
#include "menufunctions.h"
#include "colors.h"
#include "filadupla.h"
#include "terminal_utils.h"
#include "filadupla.h"

/// @brief função para o menu de atender o próximo chamado  
void menuAtenderProximoChamado(void){
    filaPrioridade* fp = estruturasGlobais.filaPrioridade;
    fila* f = estruturasGlobais.filaNormal;
    noFila* no = f->first;
    chamado chamadoAtender;
    if(fp->n > 0){
        chamadoAtender = fp->elementos[0];
    }else{
        chamadoAtender = no->chamado;
    }

    printf(GREEN"+----------------------------------------------------+"RESET);
    printf("\n");
    // titulo
    printf("%s| %s%-50s %s|\n"RESET, GREEN, BLUE, chamadoAtender.titulo, GREEN);
    
    // prioridade
    printf("%s| %sPrioridade: %s%-38d %s|\n", GREEN, MAGENTA, WHITE, chamadoAtender.prioridade, GREEN);
    
    // criador
    printf("%s| %sCriador: %s%-41s %s|\n"RESET, GREEN, MAGENTA, WHITE, chamadoAtender.criador, GREEN);
    
    // data e hora
    printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
       GREEN, MAGENTA, WHITE,
       chamadoAtender.tempoComplexo.dia, chamadoAtender.tempoComplexo.mes, chamadoAtender.tempoComplexo.ano,
       chamadoAtender.tempoComplexo.horas, chamadoAtender.tempoComplexo.minutos, chamadoAtender.tempoComplexo.segundos,
       "", GREEN);
    
       // descricao
    char descricao[7][50] = {0};
    int linhas = quebrarDescricao(chamadoAtender.descricao, descricao);
    
    //linha vazia
    printf("| %-50s |\n"RESET, "");

    // descricao
    for(int i = 0; i < linhas; i++){
        printf("%s| %s%-50s %s|\n"RESET, GREEN, CYAN, descricao[i], GREEN);
    }

    // separador
    printf(GREEN"+----------------------------------------------------+"RESET);
}

/// @brief menu para confirmar conclusao de chamado em andamento
/// @param indiceChamado indice do chamado na fila de andamento
void menuConcluirChamadoConfirmar(int indiceChamado){
    fila* fAndamento = estruturasGlobais.filaAndamento;
    noFila* no = fAndamento->first;
    for(int i = 0; i < indiceChamado; i++){
        no = no->prox;
    }
    chamado cham = no->chamado;
    printf(YELLOW"+----------------------------------------------------+"RESET);
    printf("\n");
    // titulo
    printf("%s| %s%-50s %s|\n"RESET, YELLOW, BLUE, cham.titulo, YELLOW);
    
    // prioridade
    printf("%s| %sPrioridade: %s%-38d %s|\n", YELLOW, MAGENTA, WHITE, cham.prioridade, YELLOW);
    
    // criador
    printf("%s| %sCriador: %s%-41s %s|\n"RESET, YELLOW, MAGENTA, WHITE, cham.criador, YELLOW);
    
    // data e hora
    printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
       YELLOW, MAGENTA, WHITE,
       cham.tempoComplexo.dia, cham.tempoComplexo.mes, cham.tempoComplexo.ano,
       cham.tempoComplexo.horas, cham.tempoComplexo.minutos, cham.tempoComplexo.segundos,
       "", YELLOW);

    // descricao
    char descricao[7][50] = {0};
    int linhas = quebrarDescricao(cham.descricao, descricao);

     //linha vazia
    printf("| %-50s |\n"RESET, "");

    // descricao
    for(int i = 0; i < linhas; i++){
        printf("%s| %s%-50s %s|\n"RESET, YELLOW, CYAN, descricao[i], YELLOW);
    }
   
    // separador
    printf(YELLOW"+----------------------------------------------------+"RESET);
}

/// @brief menu para confirmar exclusao de chamado fechado
/// @param indiceChamado indice do chamado na fila dupla
void menuDeletarChamadoConfirmar(int indiceChamado){
    filaDupla* fFechados = estruturasGlobais.filadupla;
    noDuplo* no = fFechados->inicio;
    for(int i = 0; i < indiceChamado; i++){
        no = no->prox;
    }
    chamado cham = no->chamado;
    printf(RED"+----------------------------------------------------+"RESET);
    printf("\n");
    // titulo
    printf("%s| %s%-50s %s|\n"RESET, RED, BLUE, cham.titulo, RED);
    
    // prioridade
    printf("%s| %sPrioridade: %s%-38d %s|\n", RED, MAGENTA, WHITE, cham.prioridade, RED);
    
    // criador
    printf("%s| %sCriador: %s%-41s %s|\n"RESET, RED, MAGENTA, WHITE, cham.criador, RED);
    
    // data e hora
    printf("%s| %sData e Hora: %s%02d/%02d/%04d %02d:%02d:%02d%-18s %s|\n",
       RED, MAGENTA, WHITE,
       cham.tempoComplexo.dia, cham.tempoComplexo.mes, cham.tempoComplexo.ano,
       cham.tempoComplexo.horas, cham.tempoComplexo.minutos, cham.tempoComplexo.segundos,
       "", RED);

    // descricao
    char descricao[7][50] = {0};
    int linhas = quebrarDescricao(cham.descricao, descricao);

     //linha vazia
    printf("| %-50s |\n"RESET, "");

    // descricao
    for(int i = 0; i < linhas; i++){
        printf("%s| %s%-50s %s|\n"RESET, RED, CYAN, descricao[i], RED);
    }
   
    // separador
    printf(RED"+----------------------------------------------------+"RESET);
}
