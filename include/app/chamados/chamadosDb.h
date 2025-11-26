#ifndef CHAMADOSDB_C
#define CHAMADOSDB_C

#include "all.h"

void carregarChamados(int type);

void adicionarChamadoTXT(const chamado *c, int type);

void deletarChamadoTXT(int id, int type);

void deletarTodosChamadosTXT(void);

#endif