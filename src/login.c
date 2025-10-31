#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "utils.h"
#include "colors.h"
#include "carregamento.h"

// funcao para printar o modelo do menu do login
static void loginHeader(int type){
    // qual tipo
    // 0 - > padrao
    // 1 -> usuario errado
    // 2 -> senha errada
    
    repetirChar(22, '=', GREEN);
    printf(GREEN"LOGIN"RESET);
    repetirChar(23, '=', GREEN);
    printf("\n");
    
    repetirChar(50, ' ', "");
    printf("\n");
    
    printf(RED"(Pressione ESC para sair)\n"RESET);
    
    repetirChar(50, ' ', "");
    printf("\n");
    
    if(type == 1){
        printf(RED"Usuario nao encontrado\n"RESET);
    }else if(type == 2){
        printf(RED"Senha incorreta\n"RESET);
    }
}

void login(int codigoErro){
    
    // parametro INT codigoErro ( funcao pode ser chamado de forma recursiva ) 
    // pode ser 0, nao tem nada errado (chamado pelo proprio main.c provavelmente)
    // 1 - usario errado 
    // 2 - senha errada
    
    const char USUARIO[50] = "mazui";
    const char senha[50] = "mazui123";
    
    char holderUsr[50];
    char holderSenha[50];
    
    loginHeader(codigoErro);
    
    printf("Usuario: ");
    fflush(stdout); // previnir que o Usuario nao seja printado
    
    int escVerification = 0; // variavel para verificar se o ESC foi indentificado no inputASCII
    inputASCII(holderUsr, 40, GREEN, &escVerification); // ler input
    
    if(escVerification){
        // se for pressionado o esc retorna para o primeiro menu
        clear();
        return;
    }
    if(!compStr(holderUsr, USUARIO)){
        // se o usuario nao for encontrado
        clear();
        login(1); // chama login denovo ( usuario errado )
        return;
    }
    clear();
    loginHeader(0); // printa o menu padrao ( sem nenhum tipo de aviso de erro )
    printf("Usuario: %s%s\n"RESET, GREEN, holderUsr);
    printf("Senha: ");
    fflush(stdout);
    
    inputASCII(holderSenha, 50, GREEN, &escVerification); // ler input
    
    if(escVerification){
        // se for pressionado o esc retorna para o primeiro menu
        clear();
        return;
    }
    if(!compStr(holderSenha, senha)){
        // se a senha estiver errada
        clear();
        login(2); // chama login denovo ( senha errado )
        return;
    }
    clear();
    carregamentoAnimacao();
    mainMenu(holderUsr);
}