#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "all.h"

#define ESTOQUE_FILE "../db/estoque.txt"

// ----------------------
// Funções auxiliares
// ----------------------

// Remove espaços no início e fim de uma string
static char* trim(char* str) {
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') str++;
    char* end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }
    return str;
}

// Cria arquivo TXT vazio se não existir
static void criarArquivoTXT(void) {
    FILE* fp = fopen(ESTOQUE_FILE, "a"); // cria se não existir
    if (!fp) {
        printf("Erro ao criar o arquivo %s\n", ESTOQUE_FILE);
        exit(1);
    }
    fclose(fp);
}

// ----------------------
// Funções principais
// ----------------------

/// @brief Carrega o estoque a partir do arquivo TXT
void carregarEstoqueTXT(void) {
    FILE* fp = fopen(ESTOQUE_FILE, "r");
    if (!fp) {
        criarArquivoTXT();
        fp = fopen(ESTOQUE_FILE, "r");
        if (!fp) exit(1);
    }

    char linha[256];

    while (fgets(linha, sizeof(linha), fp)) {
        char* l = trim(linha);

        char nome[50];
        int id, quantidade, requisicoes;

        if (sscanf(l, "%d;%49[^;];%d;%d;",
                   &id, nome, &quantidade, &requisicoes) == 4) {
            Item* item = malloc(sizeof(Item));
            item->id = id;
            item->quantidade = quantidade;
            item->requisicoes = requisicoes;
            strcpy(item->nome, nome);        
            arv_inserir(estruturasGlobais.estoque, item);
        }
    }

    fclose(fp);
}

/// @brief atualiza a quantidade de um item no estoque
/// @param itemNome nome do item a ser atualizado
/// @param novaQuantidade nova quantidade do item
void atualizarQuantidadeEstoque(char* itemNome, int novaQuantidade) {
    FILE* fp = fopen(ESTOQUE_FILE, "r");
    if (!fp) return;

    FILE* temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(fp);
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), fp)) {
        char nome[50];
        int id, quantidade, requisicoes;

        if (sscanf(linha, "%d;%49[^;];%d;%d;", &id, nome, &quantidade, &requisicoes) == 4) {
            if (strcmp(nome, itemNome) == 0) {
                // escreve a linha atualizada
                fprintf(temp, "%d;%s;%d;%d;\n", id, nome, novaQuantidade, requisicoes);
            } else {
                // escreve a linha original
                fprintf(temp, "%s", linha);
            }
        }
    }

    fclose(fp);
    fclose(temp);

    remove(ESTOQUE_FILE);
    rename("temp.txt", ESTOQUE_FILE);
    // atualiza na arvore tambem
    // buscar o item na arvore e atualizar
    Item* item;
    item = (Item*)arv_buscar(estruturasGlobais.estoque, itemNome);
    item->quantidade = novaQuantidade;
}

/// @brief Adiciona um novo item ao estoque
/// @param novoItem Ponteiro para o item a ser adicionado
void adicionarItemEstoque(Item* novoItem) {
    FILE* fp = fopen(ESTOQUE_FILE, "a");
    if (!fp) {
        criarArquivoTXT();
        fp = fopen(ESTOQUE_FILE, "a");
        if (!fp) {
            printf("Erro ao abrir o arquivo %s\n", ESTOQUE_FILE);
            exit(1);
        }
    }

    fprintf(fp, "%d;%s;%d;%d;\n",
            novoItem->id, novoItem->nome, novoItem->quantidade, novoItem->requisicoes);

    fclose(fp);
    // adicionar na arvore tambem
    arv_inserir(estruturasGlobais.estoque, novoItem);
}

/// @brief deleta um item do estoque pelo nome
/// @param itemNome nome do item a ser deletado
void deletarItemEstoque(char* itemNome) {
    FILE* fp = fopen(ESTOQUE_FILE, "r");
    if (!fp) return;

    FILE* temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(fp);
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), fp)) {
        char nome[50];
        int id, quantidade, requisicoes;

        if (sscanf(linha, "%d;%49[^;];%d;%d;", &id, nome, &quantidade, &requisicoes) == 4) {
            if (strcmp(nome, itemNome) != 0) {
                // escreve a linha original
                fprintf(temp, "%s", linha);
            }
        }
    }

    fclose(fp);
    fclose(temp);

    remove(ESTOQUE_FILE);
    rename("temp.txt", ESTOQUE_FILE);
    // deletar da arvore tambem
    arv_remover(estruturasGlobais.estoque, (void*)itemNome);
}