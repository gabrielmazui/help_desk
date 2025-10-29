// gcc main.c lib\*.c src\*.c -I"include" -o bin/main.exe
// comando para compilar no Windows

// gcc main.c lib/*.c src/*.c -Iinclude -o bin/main
// comando para compilar no Linux/macOS

#include <stdio.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h> // apenas no Windows
#endif

#include "menu.h"
#include "colors.h"
#include "utils.h"
#include "login.h"

// funcao para controlar visibilidade do cursor (apenas para windowns)
void setCursorVisible(int visible) {
    #ifdef _WIN32
        CONSOLE_CURSOR_INFO info;
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleCursorInfo(h, &info);
        info.bVisible = visible;
        SetConsoleCursorInfo(h, &info);
    #endif
}

// funcao para ficar reescrevendo as opcoes conforme o selecionado
void updateMenuSelection(int selected){
    printf("\033[2A");
    if(selected == 1){
        printf(BG_GREEN "Sim" RESET "\n");
        printf(RED "Nao" RESET "\n");
    } else {
        printf(GREEN "Sim" RESET "\n");
        printf(BG_RED "Nao" RESET "\n");
    }
}


void desenharPrimeiroMenu(void){
    printf("Oi :)\n");
        printf("Deseja fazer login?\n\n");
        // o selected inicia em 1
        printf(BG_GREEN "Sim" RESET "\n");
        printf(RED "Nao" RESET "\n");
        
}
int main(){
    
    // esconde o cursor de texto
    #ifdef _WIN32
        setCursorVisible(0); // esconde cursor no Windows
    #else
        printf("\033[?25l"); // esconde cursor no Linux/macOS
        fflush(stdout);
    #endif
    
    // esse codigo acima esconde o cursor de texto que fica piscando ( mais por questao de estetica mesmo)
        clear();
        int selected = 1;
        
    while(1){
        desenharPrimeiroMenu();
        // loop que vai ficar lendo as setas para cima e para baixo ate receber enter
        while(1){
            KeyCode teclaEscolhida = userGetKey();
            if(teclaEscolhida == KC_ENTER){
                break;
            }else if(teclaEscolhida == KC_DOWN){
                if(selected == 1){
                    selected = 2;
                    updateMenuSelection(selected);
                    continue;
                }
            }else if(teclaEscolhida == KC_UP){
                if(selected == 2){
                    selected = 1;
                    updateMenuSelection(selected);
                    continue;
                }
            }
        }
        
        if(selected == 1){
            clear();
            login(0);
        }else if(selected == 2){
            clear();
            printf(BLUE"       __\n"RESET);
            printf(BLUE"  _   / /\n"RESET);
            printf(BLUE" (_) | | \n"RESET);
            printf(BLUE"     | | \n"RESET);
            printf(BLUE"  _  | | \n"RESET);
            printf(BLUE" (_) | | \n"RESET);
            printf(BLUE"      \\_\\\n"RESET);
            break;
        }else{
            clear();
        }
    }
    #ifdef _WIN32
        setCursorVisible(1); // mostra cursor no Windows
    #else
        printf("\033[?25h"); // mostra cursor no Linux/macOS
        fflush(stdout);
    #endif
    // mostrar o cursor de texto de novo
    return 0;
}

