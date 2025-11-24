#include <stdio.h>
#include <stdlib.h>

#include "all.h"

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

    User u = {0};


    loginHeader(codigoErro);
    
    printf("Usuario: ");
    fflush(stdout); // previnir que o Usuario nao seja printado
    
    int mostrarBuffer = 1;
    while(1){
        KeyCode k = inputASCII(u.usuario, 40, GREEN, mostrarBuffer); // ler input
        if(k == KC_ESC){
            controlarCursor(0);
            clear();
            return;
        }else if(k == RESIZE_EVENT){
            esperar_tamanho_minimo(10, 60);
            if(terminalPequenoAlertado){
                terminalPequenoAlertado = 0;
                clear();
                loginHeader(codigoErro);
                printf("Usuario: ");
                mostrarBuffer = 1;
                continue;
            }
            mostrarBuffer = 0;
            continue;
        }else{
            break;
        }
    }
    codigoErro = 0; // resetar o codigo de erro para o proximo passo

    if(carregarUsuarioTXT(&u) == 0){
        // se o usuario nao for encontrado
        clear();
        login(1); // chama login denovo ( usuario errado )
        return;
    }
    clear();
    loginHeader(0); // printa o menu padrao ( sem nenhum tipo de aviso de erro )
    printf("Usuario: %s%s\n"RESET, GREEN, u.usuario);
    printf("Senha: ");
    fflush(stdout);
    mostrarBuffer = 1;
    while(1){
    KeyCode k2 = inputASCII(u.senha, 50, GREEN, mostrarBuffer); // ler input
        if(k2 == KC_ESC){
            // se for pressionado o esc retorna para o primeiro menu
            clear();
            controlarCursor(0);
            return;
        }else if(k2 == RESIZE_EVENT){
            esperar_tamanho_minimo(10, 60);
            if(terminalPequenoAlertado){
                clear();
                loginHeader(codigoErro);
                printf("Usuario: %s%s\n"RESET, GREEN, u.usuario);
                printf("Senha: ");
                fflush(stdout);
                terminalPequenoAlertado = 0;
                mostrarBuffer = 1;
                continue;
            }
            mostrarBuffer = 0;
            continue;
        }
        else {
            break;
        }
    }
    if(!validarSenhaTXT(&u)){
        // se a senha estiver errada
        clear();
        login(2); // chama login denovo ( senha errado )
        return;
    }
    clear();
    carregamentoAnimacao();
    usuario = u; // salvar o usuario logado na variavel global
    mainMenu();
}