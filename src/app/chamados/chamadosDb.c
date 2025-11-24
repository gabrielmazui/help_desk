#include <stdio.h>
#include <stdlib.h>

#include "all.h"

#define CHAMADOS_FILE1 "../db/ca1.json"
#define CHAMADOS_FILE2 "../db/ca2.json"
#define CHAMADOS_FILE3 "../db/cAnd.json"
#define CHAMADOS_FILE4 "../db/cf.json"
#define CHAMADOS_FILE5 "../db/cs.json"


static char* fileType(int type) {
    char *f;
    switch (type){
    case 1:
        f = CHAMADOS_FILE1;
        break;
    case 2:
        f = CHAMADOS_FILE2;
        break;
    case 3:
        f = CHAMADOS_FILE3;
        break;
    case 4:
        f = CHAMADOS_FILE4;
        break;
    case 5:
        f = CHAMADOS_FILE5;
        break;
    }
    return f;
}

// Cria o arquivo JSON de usuários se ele não existir
static void criarArquivoJSON(int type) {
    char *f = fileType(type);
    FILE* fp = fopen(f, "w");
    if (!fp) {
        printf("Erro ao criar o arquivo %s\n", f);
        exit(1);
    }
    fprintf(fp, "[]\n"); // JSON vazio
    fclose(fp);
}

// Remove espaços no início de uma string
static char* trim(char* str) {
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') str++;
    return str;
}


// Carrega um usuário do arquivo JSON
void carregarChamadosJSON(int type, void(*funcaoAdicionar)(chamado * c)) {
    char * f = fileType(type);
    FILE* fp = fopen(f, "r");
    if (!fp) {
        criarArquivoJSON(type);
        fp = fopen(f, "r");
        if (!fp) return 0;
    }

    char linha[1024];
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), fp)) {
        char* l = trim(linha);

        char usuarioArquivo[50], senhaArquivo[50];
        int tipo, idArquivo;
        int dia, mes, ano, horas, minutos, segundos;
        chamado * c = malloc(sizeof(chamado));
        sscanf(l,
            "{\"id\":\"%d\",\"titulo\":\"%59[^\"]\",\"descricao\":%349[^\"]\",\"prioridade\":%d,\"tempoSimples\":%ld,",
            "\"criador\":\"%49[^\"]\",\"atendente\":\"%49[^\"]\",",
            "\"criacao\":{\"dia\":%d,\"mes\":%d,\"ano\":%d,\"horas\":%d,\"minutos\":%d,\"segundos\":%d}}\n]",
            c->id, c->titulo, c->descricao, c->prioridade, c->tempoSimples,
            c->criador, c->atendente,
            c->tempoComplexo.dia, c->tempoComplexo.mes, c->tempoComplexo.ano,
            c->tempoComplexo.horas, c->tempoComplexo.minutos, c->tempoComplexo.segundos);
        funcaoAdicionar(c);
    }
    fclose(fp);
}

// Adiciona um novo usuário ao arquivo JSON
void adicionarChamadoJSON(int type, chamado * c) {
    char * f = fileType(type);
    FILE* fp = fopen(f, "r");
    if (!fp) {
        criarArquivoJSON(type);
        fp = fopen(f, "r");
        if (!fp) {
            printf("Erro ao abrir o arquivo %s\n", f);
            exit(1);
        }
    }

    // Lê todo o arquivo
    fseek(fp, 0, SEEK_END);
    long tamanho = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = malloc(tamanho + 1);
    fread(buffer, 1, tamanho, fp);
    buffer[tamanho] = '\0';
    fclose(fp);

    // Remove o último ']'
    char* p = strrchr(buffer, ']');
    if (p) *p = '\0';

    // Reabre para escrever
    fp = fopen(f, "w");
    fprintf(fp, "%s", buffer);

    // Se não estava vazio, adiciona vírgula
    if (tamanho > 3) fprintf(fp, ",\n");

    fprintf(fp,
        "    {\"id\":\"%d\",\"titulo\":\"%s\",\"descricao\":%s,\"prioridade\":%d,\"tempoSimples\":%ld,",
        "\"criador\":\"%s\",\"atendente\":\"%s\",",
        "\"criacao\":{\"dia\":%d,\"mes\":%d,\"ano\":%d,\"horas\":%d,\"minutos\":%d,\"segundos\":%d}}\n]",
        c->id, c->titulo, c->descricao, c->prioridade, c->tempoSimples,
        c->criador, c->atendente,
        c->tempoComplexo.dia, c->tempoComplexo.mes, c->tempoComplexo.ano,
        c->tempoComplexo.horas, c->tempoComplexo.minutos, c->tempoComplexo.segundos);
    fclose(fp);
    free(buffer);
}

static char* acharFimObjeto(char* inicio) {
    int contador = 0;
    char* ptr = inicio;

    while (*ptr) {
        if (*ptr == '{') contador++;
        else if (*ptr == '}') contador--;

        ptr++;

        if (contador == 0)
            break;
    }

    return ptr; // aponta para EXATAMENTE o char depois do último '}'
}


/// @brief Deleta um usuário do json pelo nome
/// @param usuarioNome Nome do usuário a ser deletado
// Deleta um usuário do arquivo JSON
void deletarChamadoJSON(int type, int id) {
    char * f = fileType(type);
    FILE* fp = fopen(f, "r");
    if (!fp) return;

    fseek(fp, 0, SEEK_END);
    long tamanho = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = malloc(tamanho + 1);
    fread(buffer, 1, tamanho, fp);
    buffer[tamanho] = '\0';
    fclose(fp);

    // Cria novo arquivo filtrando o usuário
    fp = fopen("temp.json", "w");
    fprintf(fp, "[\n");

    char* ptr = buffer;
    int primeiro = 1;

    while ((ptr = strstr(ptr, "{\"id\":")) != NULL) {
        int idTemp;

        if (sscanf(ptr, "{\"id\":\"%d\"", &idTemp) == 1) {

            if (id == idTemp) {

                if (!primeiro) fprintf(fp, ",\n");
                char* end = acharFimObjeto(ptr);

                fwrite(ptr, 1, end - ptr, fp);

                primeiro = 0;
            }
        }

        ptr++; // evita loop infinito
    }

    fprintf(fp, "\n]\n");
    fclose(fp);
    free(buffer);

    remove(f);
    rename("temp.json", f);
}