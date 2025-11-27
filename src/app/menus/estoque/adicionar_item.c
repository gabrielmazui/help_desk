#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "all.h"

void menuAdicionarItemEstoque(void){
    clear();
    repetirChar(15, '-', BLUE);
    printf(BLUE"%s"RESET, "Criar chamado");
    repetirChar(15, '-', BLUE);
    printf("\n");
    printf(GREEN" %s"RESET, "Qual o nome do item que precisa ser adicionado ao estoque?");
    printf("\n\n");
    printf(" Nome do item: ");
    char nome[50] = "";
    while(strlen(nome) == 0){
        KeyCode k = inputASCII(nome, 45, BLUE, 0);
        if(k == RESIZE_EVENT){
            if(terminalPequenoAlertado){
                terminalPequenoAlertado = 0;
                esperar_tamanho_minimo(10, 55);
                clear();
                repetirChar(15, '-', BLUE);
                printf(BLUE"%s"RESET, "Criar chamado");
                repetirChar(15, '-', BLUE);
                printf("\n");
                printf(GREEN" %s"RESET, "Qual o nome do item que precisa ser adicionado ao estoque?");
                printf("\n\n");
                printf(" Nome do item: %s", nome);
                fflush(stdout);
            }
        }else if(k == KC_ESC){
            menuHandler * m = (menuHandler*)popPilha(estruturasGlobais.pil);
            free(m);
            return;
        }
    }
    Item temp = {0};
    strcpy(temp.nome, nome);
    Item* temp2 = (Item*)arv_buscar(estruturasGlobais.estoque, &temp);
    if(temp2 == NULL){
        // caso nao encontrar o item no estoque, adicionar
        Item* item = malloc(sizeof(Item));
        item->id = itemIdCounter++;
        item->quantidade = 0;
        item->requisicoes = 0;
        strcpy(item->nome, nome);
        alterarVariavelConfig(3, itemIdCounter);
        adicionarItemEstoque(item);
    }
    menuHandler * m = (menuHandler*)popPilha(estruturasGlobais.pil);
    free(m);
}