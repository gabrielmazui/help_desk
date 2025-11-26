#ifndef ALL_H
#define ALL_H

// lib
#include "lib/pilha.h"
#include "lib/fila.h"
#include "lib/filadupla.h"
#include "lib/filaprioridade.h"
#include "lib/arvore_bin.h"


// include chamados
#include "app/chamados/chamado.h"

// include menuhandler
#include "app/menus/menu_handler.h"

// include estruturas
#include "lib/estruturas.h"

// include item
#include "app/estoque/items.h"

// include keycode
#include "app/extras/keycode.h"

//include prioridade
#include "app/extras/prioridade.h"

// db
#include "app/users/userDb.h"
#include "app/chamados/chamadosDb.h"
#include "app/estoque/estoque.h"

// config
#include "config.h"

// extras
#include "app/extras/terminal_utils.h"
#include "app/extras/utils.h"

// chamados
#include "app/chamados/extra_functions.h"
#include "app/menus/criar_chamado.h"

// visualizar chamados
#include "app/menus/visualizar_cham/admins.h"
#include "app/menus/visualizar_cham/clientes.h"
#include "app/menus/visualizar_cham/fechados.h"
#include "app/menus/visualizar_cham/atendentes.h"

// menus
#include "app/menus/login.h"
#include "app/menus/menufunctions.h"
#include "app/menus/menu.h"

// ui
#include "ui/carregamento.h"
#include "ui/colors.h"

#endif