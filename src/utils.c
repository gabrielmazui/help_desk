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

#ifdef _WIN32
#include <windows.h> // apenas no Windows
#endif

void controlarCursor(int visible) {
    // controla visibilidade do cursor de texto windows e linux/macOS
    #ifdef _WIN32
        CONSOLE_CURSOR_INFO info;
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleCursorInfo(h, &info);
        info.bVisible = visible;
        SetConsoleCursorInfo(h, &info);
    #else
        if(visible) {
            printf("\033[?25h"); // mostra cursor no Linux/macOS
        } else {
            printf("\033[?25l"); // esconde cursor no Linux/macOS
        }
        fflush(stdout);
    #endif
}

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

void inputASCII(char *buffer, int maxChars, const char *color, int *escVerification) {
    controlarCursor(1); // mostra o cursor durante a entrada
    int len = 0;
    char c;
    int currentLineChars = 0;

    while (1) {
        c = getChar();

        if (c == '\r' || c == '\n') {
            // Enter encerra apenas se algo foi digitado
            if (len == 0) {
                printf("%sTexto invalido%s\n", RED, RESET);
                printf("%s", color);
                fflush(stdout);
                continue;
            }
            break;
        } 
        else if (c == 127 || c == '\b') {
            // Backspace
            
            if (len > 0) {
                if (currentLineChars == 0) {
                    // sobe uma linha
                    printf("\033[A"); // ANSI: move o cursor uma linha pra cima
                    // vai pro fim da linha anterior
                    printf("\033[50C"); // move 50 colunas pra direita (ou o número real de chars da linha anterior)
                    fflush(stdout);
                    currentLineChars = 50; // volta pro fim da linha anterior
                }
                len--;
                buffer[len] = '\0';
                printf("\b \b");
                fflush(stdout);
                currentLineChars--;
                if (currentLineChars < 0) currentLineChars = 0;
            }
        }
        else if ((unsigned char)c == 27) {
            // ESC
            *escVerification = 1;
            break;
        } 
        else if (c >= 32 && c < 127) {
            // Só aceita caracteres visíveis
            if (len < maxChars - 1) {
                
                if (c == ' ' && len == 0)
                    continue;

                if (c == ' ' && buffer[len - 1] == ' ')
                    continue;

                buffer[len++] = c;
                printf("%s%c" RESET, color, c);
                fflush(stdout);

                currentLineChars++;

                // 🔹 quebra a linha a cada 50 caracteres
                if (currentLineChars >= 50) {
                    printf("\n%s ", color);
                    fflush(stdout);
                    currentLineChars = 0;
                }
            }
        }
    }
    buffer[len] = '\0';
    controlarCursor(0); // esconde o cursor após a entrada
}


// funcao semelhante a anterior
// porem essa le apenas numeros
void inputNumeroASCII(char *buffer, int maxChars, const char *color, int *escVerification) {
    controlarCursor(1); // mostra o cursor durante a entrada
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
    controlarCursor(0); // esconde o cursor após a entrada
    buffer[len] = '\0';
}



// funcao para inicializar todas as estruturas
void initEstruturas(Estruturas* estruturas){
    estruturas->pil = criarPilha();
    estruturas->filaNormal = criarFila();
    estruturas->filaPrioridade = criarFilaPrioridade();
    estruturas->filadupla = criarFilaDupla();
    estruturas->filaAndamento = criarFila();
}

// funcao para limpar a tela
void clear(void){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}