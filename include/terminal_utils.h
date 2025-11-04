#ifndef TERMINAL_UTILS_H
#define TERMINAL_UTILS_H

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#endif

// Variáveis globais de tamanho do terminal
#ifdef _WIN32
extern int terminal_linhas;
extern int terminal_colunas;
#else
#include <sys/ioctl.h>
extern struct winsize terminal_size;
extern volatile int resized;
#endif

// Inicializa terminal (pega tamanho inicial e registra handler)
void init_terminal();

// Espera tamanho mínimo do terminal
void esperar_tamanho_minimo(int min_linhas, int min_colunas);

// Atualiza tamanho atual (necessário para Windows)
void atualizar_tamanho();

// Retorna 1 se houve resize desde a última chamada, 0 caso contrário
int houve_resize();

#endif
