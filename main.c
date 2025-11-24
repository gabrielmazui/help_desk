// gcc main.c lib\*.c src\*.c -I"include" -o bin/main.exe
// comando para compilar no Windows

// gcc main.c lib/*.c src/*.c -Iinclude -o bin/main
// comando para compilar no Linux/macOS

#include <stdio.h>
#include <locale.h>

#include "all.h"

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
    controlarCursor(0);
    clear();
    int selected = 1; // int para guardar opcao selecionada 
    esperar_tamanho_minimo(7, 25);
    if(terminalPequenoAlertado){
        clear();
        terminalPequenoAlertado = 0;
    }
    
    while(1){
        clear();
        desenharPrimeiroMenu();
        while(1){
            KeyCode teclaEscolhida = userGetKey();
            if(teclaEscolhida == RESIZE_EVENT){
                esperar_tamanho_minimo(7, 25);
                if(terminalPequenoAlertado){
                    clear();
                    terminalPequenoAlertado = 0;
                    desenharPrimeiroMenu();
                    updateMenuSelection(selected);
                }
                
                continue;
            }
            else if(teclaEscolhida == KC_ENTER){
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
    controlarCursor(1);
    // mostrar o cursor de texto de novo
    return 0;
}

