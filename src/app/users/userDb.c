#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "all.h"

#define USUARIO_FILE "../db/users.txt"

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
    FILE* fp = fopen(USUARIO_FILE, "a"); // cria se não existir
    if (!fp) {
        printf("Erro ao criar o arquivo %s\n", USUARIO_FILE);
        exit(1);
    }
    fclose(fp);
}

// Carrega um usuário do arquivo TXT pelo nome
int carregarUsuarioTXT(User* u) {
    FILE* fp = fopen(USUARIO_FILE, "r");
    if (!fp) {
        criarArquivoTXT();
        fp = fopen(USUARIO_FILE, "r");
        if (!fp) return 0;
    }

    char linha[256];
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), fp)) {
        char* l = trim(linha);
        char usuarioArquivo[50];
        int tipo, id;
        int dia, mes, ano, horas, minutos, segundos;

        if (sscanf(l, "%49[^;];%*[^;];%d;%d;%d/%d/%d;%d:%d:%d",
                   usuarioArquivo, &tipo, &id,
                   &dia, &mes, &ano, &horas, &minutos, &segundos) == 9) {

            if (strcmp(usuarioArquivo, u->usuario) == 0) {
                u->tipo = tipo;
                u->id = id;
                u->criacao.dia = dia;
                u->criacao.mes = mes;
                u->criacao.ano = ano;
                u->criacao.horas = horas;
                u->criacao.minutos = minutos;
                u->criacao.segundos = segundos;
                encontrado = 1;
                break;
            }
        }
    }

    fclose(fp);
    return encontrado;
}

/// @brief valida a senha de um usuário
/// @param u usuario a ser validado
/// @param senha senha a ser validada
/// @return 1 se a senha for válida, 0 caso contrário
int validarSenhaTXT(User* u) {
    FILE* fp = fopen(USUARIO_FILE, "r");
    if (!fp) {
        criarArquivoTXT();
        fp = fopen(USUARIO_FILE, "r");
        if (!fp) return 0;
    }

    char linha[256];

    while (fgets(linha, sizeof(linha), fp)) {
        char* l = trim(linha);
        char usuarioArquivo[50], senhaArquivo[50];

        if (sscanf(l, "%49[^;];%49[^;];", usuarioArquivo, senhaArquivo) == 2) {

            if (strcmp(usuarioArquivo, u->usuario) == 0) {
                if (strcmp(senhaArquivo, u->senha) == 0) {
                    return 1; // senha correta
                }
            }
        }
    }

    fclose(fp);
    return 0;
}

// Adiciona um novo usuário
void adicionarUsuarioTXT(const User* u) {
    FILE* fp = fopen(USUARIO_FILE, "a");
    if (!fp) {
        criarArquivoTXT();
        fp = fopen(USUARIO_FILE, "a");
        if (!fp) {
            printf("Erro ao abrir o arquivo %s\n", USUARIO_FILE);
            exit(1);
        }
    }

    fprintf(fp, "%s;%s;%d;%d;%d/%d/%d;%d:%d:%d\n",
            u->usuario, u->senha, u->tipo, u->id,
            u->criacao.dia, u->criacao.mes, u->criacao.ano,
            u->criacao.horas, u->criacao.minutos, u->criacao.segundos);

    fclose(fp);
}

// Deleta um usuário pelo nome
void deletarUsuarioTXT(const char* usuarioNome) {
    FILE* fp = fopen(USUARIO_FILE, "r");
    if (!fp) return;

    FILE* temp = fopen("temp.txt", "w");
    if (!temp) {
        fclose(fp);
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), fp)) {
        char* l = trim(linha);
        char usuarioArquivo[50];

        if (sscanf(l, "%49[^;];", usuarioArquivo) == 1) {
            if (strcmp(usuarioArquivo, usuarioNome) != 0) {
                fprintf(temp, "%s\n", l);
            }
        }
    }

    fclose(fp);
    fclose(temp);

    remove(USUARIO_FILE);
    rename("temp.txt", USUARIO_FILE);
}

/// @brief carrega os atendentes do arquivo TXT 
void carregarAtendentesTXT(void){
    FILE* fp = fopen(USUARIO_FILE, "r");
    if (!fp) {
        criarArquivoTXT();
        fp = fopen(USUARIO_FILE, "r");
        if (!fp) return;
    }

    char linha[256];

    while (fgets(linha, sizeof(linha), fp)) {
        char* l = trim(linha);
        char usuarioArquivo[50];
        char senhaArquivo[50];
        int tipo, id;
        int dia, mes, ano, horas, minutos, segundos;

        if (sscanf(l, "%49[^;];%49[^;];%d;%d;%d/%d/%d;%d:%d:%d",
                   usuarioArquivo, senhaArquivo, &tipo, &id,
                   &dia, &mes, &ano, &horas, &minutos, &segundos) == 10) {

            if (tipo == 2) { // tipo 2 = tecnico
                User* novoAtendente = (User*)malloc(sizeof(User));
                strcpy(novoAtendente->usuario, usuarioArquivo);
                strcpy(novoAtendente->senha, senhaArquivo);
                novoAtendente->tipo = tipo;
                novoAtendente->id = id;
                novoAtendente->criacao.dia = dia;
                novoAtendente->criacao.mes = mes;
                novoAtendente->criacao.ano = ano;
                novoAtendente->criacao.horas = horas;
                novoAtendente->criacao.minutos = minutos;
                novoAtendente->criacao.segundos = segundos;

                arv_inserir(estruturasGlobais.atendentes, novoAtendente);
            }
        }
    }

    fclose(fp);
}