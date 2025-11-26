#ifndef USERDB_H
#define USERDB_H

typedef struct Tempo Tempo;
typedef struct fila fila;

typedef struct User {
    char usuario[35];
    char senha[50];
    int tipo;
    /*
     * 1 = cliente
     * 2 = tecnico
     * 3 = gerente
     * 4 = admin
     * 5 = criador
     */
    Tempo criacao;
    int id;
    fila* filaChamados; // definido na hora de carregar os chamados
    int Qchamados; // definido na hora de carregar os chamados
} User;

// ----------------------
// Funções principais
// ----------------------

// Carrega um usuário do arquivo TXT e preenche a struct
// Retorna 1 se encontrado, 0 caso contrário
int carregarUsuarioTXT(User* u);

// Valida a senha de um usuário
// Retorna 1 se a senha estiver correta, 0 caso contrário
int validarSenhaTXT(User* u);

// Adiciona um novo usuário ao arquivo TXT
void adicionarUsuarioTXT(const User* u);

// Deleta um usuário do arquivo TXT pelo nome
void deletarUsuarioTXT(const char* usuarioNome);

void carregarAtendentesTXT(void);

#endif // USER_H
