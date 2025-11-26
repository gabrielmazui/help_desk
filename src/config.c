#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "all.h"

// inicializar variaveis globais

Estruturas estruturasGlobais = {0};
int terminalPequenoAlertado = 0; // variavel global para indiciar se o a funcao tamanho minimo do terminal foi alertada
int lastSelected = 1; // opcao escolhida no menu anterior, util para as funcoes separadas
// sera necessario enviar essa informacao para as funcoes
int userIdCounter = 0;
int chamadoIdCounter = 0;
int itemIdCounter = 0;
char * atendenteAtual = "";

User usuario = {0}; // usuario logado atualmente

static int compararUsuarios(void* a, void* b) {
    User* userA = (User*)a;
    User* userB = (User*)b;
    int nmrA = userA->Qchamados;
    int nmrB = userB->Qchamados;
    if(nmrA < nmrB) return -1;
    if(nmrA > nmrB) return 1;
    return strcmp(userA->usuario, userB->usuario);
}

static int compararItems(void* a, void* b) {
    Item* itemA = (Item*)a;
    Item* itemB = (Item*)b;
    return strcmp(itemA->nome, itemB->nome);
}

static int compararChamados(void* a, void* b) {
    if(((chamado*)a)->prioridade > ((chamado*)b)->prioridade) return 1;
    if(((chamado*)a)->prioridade == ((chamado*)b)->prioridade) return (((chamado*)a)->tempoSimples < ((chamado*)b)->tempoSimples);
    return 0;
}

static void limparChamados(void* a){
    free(((chamado*)a)->quantMateriaisPorItem);
    filaLiberar(&((chamado*)a)->materiais);
    free(a);
}

static void limparUsuarios(void* a){
    filaLiberar(&((User*)a)->filaChamados);
    free(a);
}

static void limparItems(void* a){
    free(a);
}

static void limparMenus(void* a){
    free(a);
}

/// @brief muda o valor de uma variavel de configuracao
/// @param tipo 1->userIdCounter 2->chamadoIdCounter 3->itemIdCounter
/// @param novoValor valor novo da variavel
void alterarVariavelConfig(int tipo, int novoValor){
    // tipo 1 - userIdCounter
    // tipo 2 - chamadoIdCounter
    // tipo 3 - itemIdCounter

    if(tipo == 1){
        userIdCounter = novoValor;
    }else if(tipo == 2){
        chamadoIdCounter = novoValor;
    }else if(tipo == 3){
        itemIdCounter = novoValor;
    }

    // reescrever arquivo
    FILE* fp = fopen("../db/config.txt", "w");
    if(!fp){
        printf("Erro ao abrir o arquivo de config para escrita\n");
        exit(1);
    }
    fprintf(fp, "%d\n%d\n%d\n", userIdCounter, chamadoIdCounter, itemIdCounter);
    fclose(fp);
}

static void initConfigVar(void){
        // ler variaveis do arquivo config.txt
    FILE* fp = fopen("../db/config.txt", "r");
    if(!fp){
        // criar arquivo se nao existir
        fp = fopen("../db/config.txt", "w");
        if(!fp){
            printf("Erro ao criar o arquivo de config\n");
            exit(1);
        }
        fprintf(fp, "0\n0\n0\n");
        fclose(fp);
    }else{
        // linha 1 - userIdCounter
        // linha 2 - chamadoIdCounter
        // linha 3 - itemIdCounter

        fscanf(fp, "%d\n", &userIdCounter);
        fscanf(fp, "%d\n", &chamadoIdCounter);
        fscanf(fp, "%d\n", &itemIdCounter);
        fclose(fp);
    }
}

static void encontrarChamadosPorAtendente(arvNo* no){
    if(no == NULL){
        return;
    }
    noDuplo *noAtual = estruturasGlobais.chamadosAndamento->inicio;
    while(noAtual != NULL){
        chamado* c = (chamado*)noAtual->dado;
        if(c->atendente != NULL && strcmp(c->atendente, ((User*)no->dado)->usuario) == 0){
            // adicionar chamado na fila do atendente
            filaInserir(((User*)no->dado)->filaChamados, c);
            ((User*)no->dado)->Qchamados++;
        }
        noAtual = noAtual->prox;
    }
    encontrarChamadosPorAtendente(no->esq);
    encontrarChamadosPorAtendente(no->dir);
}

/// @brief Inicializa as configuracoes globais do sistema 
void initConfigs(void){
    estruturasGlobais.pil = criarPilha(limparMenus);
    estruturasGlobais.chamadosAbertosSemPrioridade = criarFila(limparChamados);
    estruturasGlobais.chamadosAbertosComPrioridade = criarFilaPrioridade(compararChamados, limparChamados);
    estruturasGlobais.chamadosAndamento = criarFilaDupla(limparChamados);
    estruturasGlobais.chamadosSuspensos = criarFilaDupla(limparChamados);
    estruturasGlobais.chamadosConcluidos = criarFila(limparChamados);
    estruturasGlobais.atendentes = arv_criar(compararUsuarios, limparUsuarios); // comparar será definido depois
    estruturasGlobais.estoque = arv_criar(compararItems, limparItems); // comparar será definido depois

    initConfigVar();
    carregarEstoqueTXT();
    carregarAtendentesTXT();
    carregarChamados(1);
    carregarChamados(2);
    carregarChamados(3);
    carregarChamados(4);
    carregarChamados(5);

    arvNo* no = estruturasGlobais.atendentes->raiz;
    // recursivo coloca ponteiro dos chamados em cada atendente
    encontrarChamadosPorAtendente(no);
}