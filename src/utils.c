#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifndef _WIN32
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#else
#include <conio.h>
#endif

#include "menu.h"
#include "colors.h"
#include "pilha.h"
#include "fila.h"
#include "filaprioridade.h"
#include "filadupla.h"


// funcao para repetir chars na tela (para deixar mais simetrico)
void repetirChar(int n, char c, char* COLOR){
    // n -> quantas vezes
    // c -> qual char para repetir
    // COLOR -> cor para printar no terminar (pode ser "" em caso de branco)
    for(int i = 0; i < n; i++){
        printf("%s%c"RESET, COLOR, c);
    }
}

// funcao para comparar 2 strings
int compStr(const char a[], const char b[]){
    // char a , char b -> strings para comparar (constantes pois nao vao ser alteradas) 
    int i = 0;
    while(a[i] != '\0'){
        if(a[i] != b[i]){
            return 0;
        }
        i++;
    }
    return a[i] == b[i];
}

// funcao para windowns e linux/macos conseguirem ler
// char sem enter no terminal
char getChar(void) {
#ifdef _WIN32
    return _getch();
#else
    char c;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    read(STDIN_FILENO, &c, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
#endif
}

// usa o getChar
// funcao para ler as teclas (esc, seta, enter)
// meio complicado a tecla ESC, pois as setas em si comecam com ESC
// o ESC sozinho e 27 na tabela ASCII
// e as setas tambem comecam com isso
KeyCode userGetKey(void) {
#ifdef _WIN32
    char c = getChar();

    if (c == 0 || c == -32) { // tecla especial no Windows
        char arrow = getChar();
        switch (arrow) {
            case 72: return KC_UP;
            case 80: return KC_DOWN;
            case 75: return KC_LEFT;
            case 77: return KC_RIGHT;
            default: return KC_OTHER;
        }
    } else if (c == 27) {
        return KC_ESC;
    } else if (c == '\r') {
        return KC_ENTER;
    }
    return KC_OTHER;

#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    unsigned char c = 0;
    read(STDIN_FILENO, &c, 1);

    if (c == 27) { // ESC ou sequência ANSI
        usleep(5000); // dá tempo pros bytes chegarem (5 ms)
        int bytesWaiting = 0;
        ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);

        if (bytesWaiting >= 2) {
            char seq[2];
            read(STDIN_FILENO, &seq, 2);
            if (seq[0] == '[') {
                switch (seq[1]) {
                    case 'A': tcsetattr(STDIN_FILENO, TCSANOW, &oldt); return KC_UP;
                    case 'B': tcsetattr(STDIN_FILENO, TCSANOW, &oldt); return KC_DOWN;
                    case 'C': tcsetattr(STDIN_FILENO, TCSANOW, &oldt); return KC_RIGHT;
                    case 'D': tcsetattr(STDIN_FILENO, TCSANOW, &oldt); return KC_LEFT;
                }
            }
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return KC_ESC; // ESC isolado
    } 
    else if (c == '\n' || c == '\r') {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return KC_ENTER;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return KC_OTHER;
#endif
}

// Função de input limitado a chars de 1 byte
void inputASCII(char *buffer, int maxChars, const char *color, int * escVerification) {
    
    // buffer -> vai guardando os char nessa string 
    // maxChars -> quantidade maxima de caracteres que podem ser lidas
    // color -> cor para ser mostrado o caractere escrito
    // escVerification - > 0 por padrao, vira 1 caso for pressionado 
    
    int len = 0;
    char c;

    while (1) {
        c = getChar();

        if (c == '\r' || c == '\n') {
            // Enter pressionado
            break; // termina input
        } 
        else if (c == 127 || c == '\b') {
            // Backspace
            if (len > 0) {
                len--; // remove último caractere do buffer
                buffer[len] = '\0'; 
                printf("\b \b"); // remove do terminal
                fflush(stdout);
            }
        }else if ((unsigned char)c == 27) { 
            // ESC
            *escVerification = 1;
            break;
        } 
        else if (c < 127) {
            // caractere normal
            buffer[len++] = c;
            printf("%s%c"RESET, color , c);
            fflush(stdout);
        }
    
        // caracteres > 127 são ignorados ( alem do backspace)
    }

    buffer[len] = '\0';
}

// funcao semelhante a anterior
// porem essa le apenas numeros
void inputNumeroASCII(char *buffer, int maxChars, const char *color, int *escVerification) {
    int len = 0;
    char c;

    while (1) {
        c = getChar(); // sua função de input

        if (c == '\r' || c == '\n') {
            break; // Enter
        } 
        else if (c == 127 || c == '\b') { // Backspace
            if (len > 0) {
                len--;
                buffer[len] = '\0';
                printf("\b \b");
                fflush(stdout);
            }
        } 
        else if ((unsigned char)c == 27) { // ESC
            *escVerification = 1;
            break;
        } 
        else if (c >= '0' && c <= '9') { // aceita apenas dígitos
            if (len < maxChars) {
                buffer[len++] = c;
                printf("%s%c" RESET, color, c);
                fflush(stdout);
            }
        }
        // qualquer outro caractere é ignorado
    }

    buffer[len] = '\0';
}



// funcao para inicializar todas as estruturas
void initEstruturas(Estruturas* estruturas){
    estruturas->pil = criarPilha();
    estruturas->filaNormal = criarFila();
    estruturas->filaPrioridade = criarFilaPrioridade();
    estruturas->filadupla = criarFilaDupla();
}

// funcao para limpar a tela
void clear(void){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}