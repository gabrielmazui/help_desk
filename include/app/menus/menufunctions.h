#ifndef MENUFUNCTIONS_H
#define MENUFUNCTIONS_H

#include <time.h>

#include "all.h"

menuHandler* createMenu(int type);

void handlerMenuSelect(int type, int selected, pilha** p);

void menuHandlerEsc(int type, pilha * p);

#endif