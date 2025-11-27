#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "all.h"

#define CHAMADO1_FILE "../db/ca1.txt"
#define CHAMADO2_FILE "../db/ca2.txt"
#define CHAMADO3_FILE "../db/cAnd.txt"
#define CHAMADO4_FILE "../db/cf.txt"
#define CHAMADO5_FILE "../db/cs.txt"

static void criarArquivoChamados(int type) {
    char file_name[50];
    switch(type){
        case 1: strcpy(file_name, CHAMADO1_FILE); break;
        case 2: strcpy(file_name, CHAMADO2_FILE); break;
        case 3: strcpy(file_name, CHAMADO3_FILE); break;
        case 4: strcpy(file_name, CHAMADO4_FILE); break;
        case 5: strcpy(file_name, CHAMADO5_FILE); break;
    }
    FILE *fp = fopen(file_name, "a");
    if (!fp) {
        printf("Erro ao criar arquivo de chamados!\n");
        exit(1);
    }
    fclose(fp);
}

static void limparItems(void* a){
    free(a);
}

void carregarChamados(int type){
    char file_name[50];
    switch(type){
        case 1: strcpy(file_name, CHAMADO1_FILE); break;
        case 2: strcpy(file_name, CHAMADO2_FILE); break;
        case 3: strcpy(file_name, CHAMADO3_FILE); break;
        case 4: strcpy(file_name, CHAMADO4_FILE); break;
        case 5: strcpy(file_name, CHAMADO5_FILE); break;
    }
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        criarArquivoChamados(type);
        return;
    }

    while (1) {
        chamado *c = malloc(sizeof(chamado));
        if (!c) { printf("Erro de alocacao de memoria para chamado\n"); exit(1); }

        if (fscanf(fp, "%d\n", &c->id) != 1) {
            free(c);
            break;
        }

        fgets(c->titulo, sizeof(c->titulo), fp);
        c->titulo[strcspn(c->titulo, "\n")] = 0;

        fgets(c->descricao, sizeof(c->descricao), fp);
        c->descricao[strcspn(c->descricao, "\n")] = 0;

        fscanf(fp, "%d\n", &c->prioridade);

        fgets(c->criador, sizeof(c->criador), fp);
        c->criador[strcspn(c->criador, "\n")] = 0;

        fgets(c->atendente, sizeof(c->atendente), fp);
        c->atendente[strcspn(c->atendente, "\n")] = 0;

        fscanf(fp, "%ld\n", &c->tempoSimples);

        fscanf(fp, "%d %d %d %d %d %d\n",
            &c->tempoComplexo.dia,
            &c->tempoComplexo.mes,
            &c->tempoComplexo.ano,
            &c->tempoComplexo.horas,
            &c->tempoComplexo.minutos,
            &c->tempoComplexo.segundos);

        fscanf(fp, "%d\n", &c->quantMateriais);

        c->materiais = criarFila(limparItems);

        c->quantMateriaisPorItem = malloc(sizeof(int) * c->quantMateriais);
        if (!c->quantMateriaisPorItem) exit(1);

        for (int i = 0; i < c->quantMateriais; i++) {
            char material[100];
            fgets(material, sizeof(material), fp);
            material[strcspn(material, "\n")] = 0;

            int qnt;
            fscanf(fp, "%d\n", &qnt);
            c->quantMateriaisPorItem[i] = qnt;

            Item *tmp = malloc(sizeof(Item));
            strcpy(tmp->nome, material);
            Item *buscado = arv_buscar(estruturasGlobais.estoque, tmp);
            filaInserir(c->materiais, buscado);
            free(tmp);
        }

        fscanf(fp, "%d\n", &c->cancelado);

        char sep[10];
        fgets(sep, sizeof(sep), fp);

        switch(type){
            case 1: filaPrioridadeInserir(estruturasGlobais.chamadosAbertosComPrioridade, c); break;
            case 2: filaInserir(estruturasGlobais.chamadosAbertosSemPrioridade, c); break;
            case 3: filaDuplaInserir(estruturasGlobais.chamadosAndamento, 0, c); break;
            case 4: filaInserir(estruturasGlobais.chamadosConcluidos, c); break;
            case 5: filaDuplaInserir(estruturasGlobais.chamadosSuspensos, 0, c); break;
        }
    }

    fclose(fp);
}

/// @brief adiciona um chamado ao arquivo de chamados
/// @param c chamado a ser adicionado
/// @param type 1-aberto com prioridade, 2-aberto sem prioridade, 3-andamento, 4-concluido, 5-suspenso
void adicionarChamadoTXT(const chamado *c, int type) {
    char file_name[50];
    switch(type){
        case 1: strcpy(file_name, CHAMADO1_FILE); break;
        case 2: strcpy(file_name, CHAMADO2_FILE); break;
        case 3: strcpy(file_name, CHAMADO3_FILE); break;
        case 4: strcpy(file_name, CHAMADO4_FILE); break;
        case 5: strcpy(file_name, CHAMADO5_FILE); break;
    }

    FILE *fp = fopen(file_name, "a");
    if (!fp) { criarArquivoChamados(type); fp = fopen(file_name, "a"); }

    fprintf(fp, "%d\n", c->id);
    fprintf(fp, "%s\n", c->titulo);
    fprintf(fp, "%s\n", c->descricao);
    fprintf(fp, "%d\n", c->prioridade);
    fprintf(fp, "%s\n", c->criador);
    fprintf(fp, "%s\n", c->atendente);
    fprintf(fp, "%ld\n", c->tempoSimples);

    fprintf(fp, "%d %d %d %d %d %d\n",
        c->tempoComplexo.dia,
        c->tempoComplexo.mes,
        c->tempoComplexo.ano,
        c->tempoComplexo.horas,
        c->tempoComplexo.minutos,
        c->tempoComplexo.segundos);

    fprintf(fp, "%d\n", c->quantMateriais);

    noFila *no = c->materiais->first;
    for (int i = 0; i < c->quantMateriais; i++) {
        Item *item = (Item*) no->dado;
        fprintf(fp, "%s\n", item->nome);
        fprintf(fp, "%d\n", c->quantMateriaisPorItem[i]);
        no = no->prox;
    }

    fprintf(fp, "%d\n", c->cancelado);
    fprintf(fp, "===\n");

    fclose(fp);
}

/// @brief Deleta um chamado do arquivo de chamados
/// @param id ID do chamado a ser deletado
/// @param type Tipo do chamado (1-aberto com prioridade, 2-aberto sem prioridade, 3-andamento, 4-concluido, 5-suspenso)
void deletarChamadoTXT(int id, int type) {
    char file_name[50];
    switch(type){
        case 1: strcpy(file_name, CHAMADO1_FILE); break;
        case 2: strcpy(file_name, CHAMADO2_FILE); break;
        case 3: strcpy(file_name, CHAMADO3_FILE); break;
        case 4: strcpy(file_name, CHAMADO4_FILE); break;
        case 5: strcpy(file_name, CHAMADO5_FILE); break;
    }

    FILE *fp = fopen(file_name, "r");
    if (!fp) return;

    FILE *temp = fopen("../db/temp.txt", "w");
    if (!temp) { fclose(fp); return; }

    char linha[400];
    int lendoChamado = 0;
    int ignorar = 0;
    int idLido = -1;

    while (fgets(linha, sizeof(linha), fp)) {
        // Está no início de um bloco
        if (!lendoChamado) {

            // pular whitespace à esquerda
            char *p = linha;
            while (*p == ' ' || *p == '\t' || *p == '\r') p++;

           
            if (*p < '0' || *p > '9') {
                fprintf(temp, "%s", linha);
                continue;
            }

            // É ID
            idLido = strtol(p, NULL, 10);
            lendoChamado = 1;
            ignorar = (idLido == id);

            if (!ignorar)
                fprintf(temp, "%s", linha);

            continue;
        }

        // dentro de um bloco até encontrar "==="
        if (strncmp(linha, "===", 3) == 0) {

            if (!ignorar)
                fprintf(temp, "%s", linha);

            lendoChamado = 0;
            continue;
        }

        // linhas internas
        if (!ignorar)
            fprintf(temp, "%s", linha);
    }

    fclose(fp);
    fclose(temp);

    remove(file_name);
    rename("../db/temp.txt", file_name);
}


void deletarTodosChamadosTXT(void){
    FILE *temp = fopen("../db/temp.txt", "w");
    fclose(temp);
    remove(CHAMADO4_FILE);
    rename("../db/temp.txt", CHAMADO4_FILE);
}