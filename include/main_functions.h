#ifndef MAIN_FUNCTIONS_H
#define MAIN_FUNCTIONS_H

void menuCriarChamadoHandler(int prioridade, int comPreferencia, char* user, char titulo[50], char descricao[350], int pularPrioridade);

void menuVisualizarChamados(void);

void menuHandlerConcluirChamado(void);

void menuHandlerDeletarChamado(void);

#endif