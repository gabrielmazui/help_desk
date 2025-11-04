#include "terminal_utils.h"
#include <stdio.h>
#include <unistd.h>
#include "colors.h"
#include "utils.h"

#ifdef _WIN32
#include <windows.h>
int terminal_linhas = 0;
int terminal_colunas = 0;
static int last_linhas = 0;
static int last_colunas = 0;

void atualizar_tamanho() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    terminal_linhas = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    terminal_colunas = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void init_terminal() {
    atualizar_tamanho();
    last_linhas = terminal_linhas;
    last_colunas = terminal_colunas;
}

void esperar_tamanho_minimo(int min_linhas, int min_colunas) {
    int linhas_anteriores = -1;
    int colunas_anteriores = -1;

    while (1) {
        atualizar_tamanho();

        if (terminal_linhas >= min_linhas && terminal_colunas >= min_colunas)
            break;

        // Só redesenha se o tamanho mudou desde a última verificação
        if (terminal_linhas != linhas_anteriores || terminal_colunas != colunas_anteriores) {
            terminalPequenoAlertado = 1;
            printf("\033[H\033[J"); // limpa
            printf(RED"Aumente o tamanho do terminal"RESET);
            fflush(stdout);

            linhas_anteriores = terminal_linhas;
            colunas_anteriores = terminal_colunas;
        }

#ifdef _WIN32
        Sleep(200);
#else
        usleep(200000);
#endif
    }
}

int houve_resize() {
    atualizar_tamanho();
    if (terminal_linhas != last_linhas || terminal_colunas != last_colunas) {
        last_linhas = terminal_linhas;
        last_colunas = terminal_colunas;
        return 1;
    }
    return 0;
}

#else // Linux / POSIX

struct winsize terminal_size;
volatile int resized = 0;
static int last_linhas = 0;
static int last_colunas = 0;

// Handler de SIGWINCH
static void handle_resize(int sig) {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size);
    resized = 1;
}

void atualizar_tamanho() {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size);
}

void init_terminal() {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size);
    last_linhas = terminal_size.ws_row;
    last_colunas = terminal_size.ws_col;
    signal(SIGWINCH, handle_resize);
}

void esperar_tamanho_minimo(int min_linhas, int min_colunas) {
    int ult_linhas = -1;
    int ult_colunas = -1;

    while (1) {
        atualizar_tamanho();
        int linhas = terminal_size.ws_row;
        int colunas = terminal_size.ws_col;

        if (linhas >= min_linhas && colunas >= min_colunas)
            break;

        // Só redesenha se o tamanho mudou
        if (linhas != ult_linhas || colunas != ult_colunas) {
            printf("\033[H\033[J"); // limpa a tela
            printf("Aumente o tamanho do terminal para pelo menos %dx%d\n",
                   min_colunas, min_linhas);
            printf("Tamanho atual: %dx%d\n", colunas, linhas);
            fflush(stdout);

            ult_linhas = linhas;
            ult_colunas = colunas;
        }

        usleep(200000); // 200ms
    }
}

int houve_resize() {
    if (resized) {
        last_linhas = terminal_size.ws_row;
        last_colunas = terminal_size.ws_col;
        resized = 0;
        return 1;
    }
    return 0;
}

#endif
