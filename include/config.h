#ifndef CONFIG_H
#define CONFIG_H

#include "all.h"

// =================================================================
// VARIAVEIS GLOBAIS
// =================================================================

// variavel global para indiciar se o a funcao tamanho minimo do terminal foi alertada
extern int terminalPequenoAlertado;

//ultima opcao selecionada no menu anterior
extern int lastSelected;

extern int userIdCounter; // contador de IDs para usuarios

extern int chamadoIdCounter; // contador de IDs para chamados

extern int itemIdCounter; // contador de IDs para itens do estoque

extern int tipoDeMenuVariavel; // variavel global para indicar o tipo de menu que precisa ser guardado

// usuario logado atualmente
extern User* usuario; 

// forward declaration
typedef struct Estruturas Estruturas;

// definir estruturas globais
extern Estruturas estruturasGlobais;

void alterarVariavelConfig(int tipo, int novoValor);

void initConfigs(void);

#endif