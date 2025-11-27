#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "all.h"


static void bufferRecursivo(char ***buffer, arvNo *Atual, int *index){
    if (Atual == NULL) return;

    bufferRecursivo(buffer, Atual->esq, index);

    // 2) processar nó (apenas se for 0 0)
    Item *it = (Item*)Atual->dado;
    if (it->quantidade == 0 && it->requisicoes == 0){
        // realoca buffer caso não seja o primeiro elemento
        if (*index > 0){
            *buffer = realloc(*buffer, (*index + 1) * sizeof(char*));
        }

        (*buffer)[*index] = malloc(100);
        snprintf((*buffer)[*index], 100, "%s", it->nome);
        (*index)++;
    }
    bufferRecursivo(buffer, Atual->dir, index);
}

static void carregarBufferMateriais(char ***buffer, int *paginas, int **quantOptions){
    arvoreBinaria *estoque = estruturasGlobais.estoque;

    int index = 0;
    *buffer = malloc(sizeof(char*));
    bufferRecursivo(buffer, estoque->raiz, &index);
    int q = index;

    *paginas = 0;
    *quantOptions = malloc(sizeof(int) * ((q / 10) + 1));

    while (q > 0){
        (*paginas)++;
        if (q >= 10){
            (*quantOptions)[*paginas - 1] = 10;
            q -= 10;
        } else {
            (*quantOptions)[*paginas - 1] = q;
            q = 0;
        }
    }
}


/// @brief função para printar o buffer de materiais na tela
/// @param buffer buffer de materiais
/// @param paginas quantas paginas existem
/// @param paginaAtual pagina atual que está sendo exibida
/// @param selected opcao selecionada atualmente
/// @param quantOptions quantidade de opções (materiais) por página
static void printarBufferMateriais(char **buffer, int paginas, int paginaAtual, int selected, int* quantOptions){
    clear();
    repetirChar(15, '-', BLUE);
    printf(BLUE"%s"RESET, "Deletar item inutilizado do estoque");
    repetirChar(15, '-', BLUE);
    printf("\n");
    printf(" %sEscolha o item que deseja deletar"RESET, GREEN);
    printf("\n\n");
    char* options[2] = {"Voltar", "Sair"};
    for(int i = 0; i < 2; i++){
        if(selected == i + 1){
            printf(" %s%s"RESET, BG_BLUE, options[i]);
        }else{
            printf(" %s%s"RESET, BLUE, options[i]);
        }
        printf("\n");
    }
    printf("\n");
    int len = strlen("Deletar item inutilizado do estoque");
    repetirChar(len + 30, '-', BLUE);
    // 6 linhas de header por pagina
    printf("\n");
    printf("\n\n");

    for(int i = 0; i < quantOptions[paginaAtual - 1]; i++){
        if(selected == i + 2){
            printf(" %s%s"RESET, BG_BLUE, buffer[i + (paginaAtual -1)*10]);
        }else{
            printf(" %s%s"RESET, BLUE, buffer[i + (paginaAtual -1)*10]);
        }
        printf("\n");
    }
    // no max 10 linhas por pagina de materiais
    int q = 24 - quantOptions[paginaAtual - 1] - 9;
    for(int i = 0; i < q; i++){
        printf("\n");
    }
    if(selected ==  2 + quantOptions[paginaAtual - 1]){
        printf(" %s< Pagina %d/%d >"RESET, BG_BLUE, paginaAtual, (paginas) ? paginas : 1);
    }else{
        printf(" %s< Pagina %d/%d >"RESET, BLUE, paginaAtual, (paginas) ? paginas : 1);
    }
    fflush(stdout);
}

/// @brief Função para o menu de deletar item inutilizado do estoque 
void menuDeletarItem(void){
    esperar_tamanho_minimo(25, 45);

    char **buffer = NULL;
    int* quantOptions = NULL;
    int paginas = 0;
    int selected = 1;
    carregarBufferMateriais(&buffer, &paginas, &quantOptions);
    int paginaAtual = 1;
    printarBufferMateriais(buffer, paginas, paginaAtual, selected, quantOptions);
    char* options[2] = {"Voltar", "Sair"};
    while(1){
        KeyCode k = userGetKey();
        if(k == RESIZE_EVENT){
            esperar_tamanho_minimo(25, 55);
            if(terminalPequenoAlertado){
                terminalPequenoAlertado = 0;
                clear();
                printarBufferMateriais(buffer, paginas, paginaAtual, selected, quantOptions);
            }
            continue;
        }else if(k == KC_ESC){
            menuHandler* m = (menuHandler*)popPilha(estruturasGlobais.pil);
            free(m);
            return;
        }if(k == KC_UP && selected > 1){
            if(selected == 2){
                updateOption(5, options[1], "", BLUE);
                selected--;
                updateOption(4, options[0], BG_BLUE, "");
            }else if(selected == 3){
                if(quantOptions[paginaAtual - 1] > 0){
                    updateOption(selected + 7, buffer[(selected - 3) + (paginaAtual-1)*10], "", BLUE);
                    selected--;
                    updateOption(5, options[1], BG_BLUE, "");
                }else{
                    char txt[100];
                    snprintf(txt, sizeof(txt), "< Pagina %d/%d >", paginaAtual, (paginas) ? paginas : 1);
                    updateOption(25, txt, "", BLUE);
                    selected--;
                    updateOption(5, options[1], BG_BLUE, "");
                }
            }else{
                if(selected == quantOptions[paginaAtual - 1] + 3){
                    char txt[100];
                    snprintf(txt, sizeof(txt), "< Pagina %d/%d >", paginaAtual, (paginas) ? paginas : 1);
                    updateOption(25, txt, "", BLUE);
                    selected--;
                    updateOption(selected + 7, buffer[(selected - 3) + (paginaAtual-1)*10], BG_BLUE, "");
                }else{
                    updateOption(selected + 7, buffer[(selected - 3) + (paginaAtual-1)*10], "", BLUE);
                    selected--;
                    updateOption(selected + 7, buffer[(selected - 3) + (paginaAtual-1)*10], BG_BLUE, "");
                }
            }
        }
        else if(k == KC_DOWN && selected < quantOptions[paginaAtual - 1] + 3){
            if(selected == 1){
                updateOption(4, options[0], "", BLUE);
                selected++; // desce
                // Coloca destaque na nova opção
                updateOption(5, options[1], BG_BLUE, "");
            }
            else if(selected == 2){
                if(quantOptions[paginaAtual - 1] > 0){
                    updateOption(5, options[1], "", BLUE);
                    selected++; // desce
                    // Coloca destaque na nova opção
                    updateOption(selected + 7, buffer[(selected - 3) + (paginaAtual-1)*10], BG_BLUE, "");
                }else{
                    updateOption(5, options[1], "", BLUE);
                    selected++;
                    char txt[100];
                    snprintf(txt, sizeof(txt), "< Pagina %d/%d >", paginaAtual, (paginas) ? paginas : 1);
                    updateOption(25, txt, BG_BLUE, "");
                }
            }else{
                if(selected == quantOptions[paginaAtual - 1] + 2){
                    updateOption(selected + 7, buffer[(selected - 3) + (paginaAtual-1)*10], "", BLUE);
                    selected++;
                    char txt[100];
                    snprintf(txt, sizeof(txt), "< Pagina %d/%d >", paginaAtual, (paginas) ? paginas : 1);
                    updateOption(25, txt, BG_BLUE, "");
                }else{
                    updateOption(selected + 7, buffer[(selected - 3) + (paginaAtual-1)*10], "", BLUE);
                    selected++;
                    updateOption(selected + 7, buffer[(selected - 3) + (paginaAtual-1)*10], BG_BLUE, "");
                }
            }
            
        }else if(k == KC_LEFT){
            if(selected == quantOptions[paginaAtual - 1] + 3){
                // mudar pagina para a esquerda
                if(paginaAtual > 1){
                    paginaAtual--;
                    selected = 2 + quantOptions[paginaAtual - 1];
                    printarBufferMateriais(buffer, paginas, paginaAtual, selected, quantOptions);
                }
            }
        }else if(k == KC_RIGHT){
            if(selected == quantOptions[paginaAtual - 1] + 3){
                // mudar pagina para a direita
                if(paginaAtual < paginas){
                    paginaAtual++;
                    selected = 2 + quantOptions[paginaAtual - 1];
                    printarBufferMateriais(buffer, paginas, paginaAtual, selected, quantOptions);
                }
            }
        }else if(k == KC_ENTER){
            if(selected == 1){
                menuHandler * m = (menuHandler*)popPilha(estruturasGlobais.pil);
                free(m);
                free(quantOptions);
                for(int i = 0; i < paginas; i++){
                    free(buffer[i]);
                }
                free(buffer);
                menuHandler* temp = (menuHandler*)popPilha(estruturasGlobais.pil);
                free(temp);
                return;
            }else if(selected == 2){
                // sair
                menuHandler* m = createMenu(9);
                pushPilha(estruturasGlobais.pil, m);
                free(quantOptions);
                for(int i = 0; i < paginas; i++){
                    free(buffer[i]);
                }
                free(buffer);
                menuHandler* temp = (menuHandler*)popPilha(estruturasGlobais.pil);
                free(temp);
                return;
            }else if(selected > 2 && selected < quantOptions[paginaAtual - 1] + 3){
                lastSelected = (selected - 3) + (paginaAtual - 1) * 10;
                menuHandler* m = (menuHandler*)createMenu(36);
                pushPilha(estruturasGlobais.pil, m);
                // liberar ponteiros
                free(quantOptions);
                for(int i = 0; i < paginas; i++){
                    free(buffer[i]);
                }
                free(buffer);
                break;
            }
        }
    }
}