#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "all.h"


static void bufferRecursivo(char ***buffer, arvNo*Atual, int* index){
    if(Atual == NULL) return;
    char nome[50];
    if(*index > 0){
        *buffer = realloc(*buffer, ((*index) + 1) * sizeof(char*));
        (*buffer)[*index] = malloc(100 * sizeof(char));
    }
    strcpy(nome, ((Item*)(Atual->dado))->nome);
    snprintf((*buffer)[*index],100, "%s < %d > - Requisicoes: %d", nome, ((Item*)(Atual->dado))->quantidade, ((Item*)(Atual->dado))->requisicoes);
    (*index)++;
    bufferRecursivo(buffer, Atual->esq, index);
    bufferRecursivo(buffer, Atual->dir, index);
}

static void carregarBufferMateriais(char ***buffer, int *paginas, int **quantOptions){
    // carrega os materiais da fila para o buffer
    int q = 0;
    arvoreBinaria* estoque = estruturasGlobais.estoque;
    arvNo* atual = estoque->raiz;
    int index = 0;
    *buffer = malloc(sizeof(char*));
    (*buffer)[index] = malloc(100 * sizeof(char));
    bufferRecursivo(buffer, atual, &index);
    // primeira opcao deve levar para funcao separada
    // alocar variaveis dinamicamente
    q = index;
    *quantOptions = malloc(sizeof(int) * ((q /10) + 1));
    (*quantOptions)[0] = 0;
    while(q > 0){
        (*paginas)++;
        if(q >= 10){
            (*quantOptions)[*paginas - 1] = 10;
            q -= 10;
        }else{
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
    printf(BLUE"%s"RESET, "Criar chamado");
    repetirChar(15, '-', BLUE);
    printf("\n");
    printf(" %sEscolha os materiais que deseja atualizar"RESET, GREEN);
    printf("\n\n");
    char* options[1] = {"Concluir"};
    if(selected == 1){
        printf(" %s%s"RESET, BG_BLUE, options[0]);
    }else{
        printf(" %s%s"RESET, BLUE, options[0]);
    }
    printf("\n");
    repetirChar(43, '-', BLUE);
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
    int q = 25 - quantOptions[paginaAtual - 1] - 8;
    for(int i = 0; i < q; i++){
        printf("\n");
    }
    if(selected ==  2 + quantOptions[paginaAtual - 1]){
        printf(" %s< Pagina %d/%d >"RESET, BG_BLUE, paginaAtual, paginas);
    }else{
        printf(" %s< Pagina %d/%d >"RESET, BLUE, paginaAtual, paginas);
    }
    fflush(stdout);
}

/// @brief Função para o menu de atualizar estoque 
void menuAtualizarEstoque(void){
    clear();
    repetirChar(15, '-', BLUE);
    printf(BLUE"%s"RESET, "Atualizar Estoque");
    repetirChar(15, '-', BLUE);
    printf("\n\n");

    esperar_tamanho_minimo(25, 57);
    // parte 4
    // materiais necessarios
    char **buffer = NULL;
    int* quantOptions = NULL;
    int paginas = 0;
    int selected = 1;
    carregarBufferMateriais(&buffer, &paginas, &quantOptions);
    int paginaAtual = 1;
    printarBufferMateriais(buffer, paginas, paginaAtual, selected, quantOptions);
    char* options[1] = {"Concluir"};
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
                if(quantOptions[paginaAtual - 1] > 0){
                    updateOption(selected + 6, buffer[(selected - 2) + (paginaAtual-1)*10], "", BLUE);
                    selected--;
                    updateOption(4, options[0], BG_BLUE, "");
                }else{
                    char txt[100];
                    snprintf(txt, sizeof(txt), "< Pagina %d/%d >", paginaAtual, paginas);
                    updateOption(25, txt, "", BLUE);
                    selected--;
                    updateOption(4, options[0], BG_BLUE, "");
                }
            }else{
                if(selected == quantOptions[paginaAtual - 1] + 2){
                    char txt[100];
                    snprintf(txt, sizeof(txt), "< Pagina %d/%d >", paginaAtual, paginas);
                    updateOption(25, txt, "", BLUE);
                    selected--;
                    updateOption(selected + 6, buffer[(selected - 2) + (paginaAtual-1)*10], BG_BLUE, "");
                }else{
                    updateOption(selected + 6, buffer[(selected - 2) + (paginaAtual-1)*10], "", BLUE);
                    selected--;
                    updateOption(selected + 6, buffer[(selected - 2) + (paginaAtual-1)*10], BG_BLUE, "");
                }
            }
        }
        else if(k == KC_DOWN && selected < quantOptions[paginaAtual - 1] + 2){
            if(selected == 1){
                if(quantOptions[paginaAtual - 1] > 0){
                    updateOption(4, options[0], "", BLUE);
                    selected++; // desce
                    // Coloca destaque na nova opção
                    updateOption(selected + 6, buffer[(selected - 2) + (paginaAtual-1)*10], BG_BLUE, "");
                }else{
                    updateOption(4, options[0], "", BLUE);
                    selected++;
                    char txt[100];
                    snprintf(txt, sizeof(txt), "< Pagina %d/%d >", paginaAtual, paginas);
                    updateOption(25, txt, BG_BLUE, "");
                }
            }else{
                if(selected == quantOptions[paginaAtual - 1] + 1){
                    updateOption(selected + 6, buffer[(selected - 2) + (paginaAtual-1)*10], "", BLUE);
                    selected++;
                    char txt[100];
                    snprintf(txt, sizeof(txt), "< Pagina %d/%d >", paginaAtual, paginas);
                    updateOption(25, txt, BG_BLUE, "");
                }else{
                    updateOption(selected + 6, buffer[(selected - 2) + (paginaAtual-1)*10], "", BLUE);
                    selected++;
                    updateOption(selected + 6, buffer[(selected - 2) + (paginaAtual-1)*10], BG_BLUE, "");
                }
            }
            
        }else if(k == KC_LEFT){
            if(selected == quantOptions[paginaAtual - 1] + 2){
                // mudar pagina para a esquerda
                if(paginaAtual > 1){
                    paginaAtual--;
                    selected = 2 + quantOptions[paginaAtual - 1];
                    printarBufferMateriais(buffer, paginas, paginaAtual, selected, quantOptions);
                }
            }else if(selected > 1){
                char nome[50];
                int quantidade;
                int requisicoes;
                if (sscanf(buffer[selected - 2 + (paginaAtual -1)*10], "%50[^<] < %d > - Requisicoes: %d", nome, &quantidade, &requisicoes) == 3){
                    int len = strlen(nome);
                    nome[len-1] = '\0';
                    if(quantidade > 0){
                        quantidade--;
                        snprintf(buffer[selected - 2 + (paginaAtual -1)*10], 100, "%s < %d > - Requisicoes: %d", nome, quantidade, requisicoes);
                        updateOption(selected + 6, buffer[selected - 2 + (paginaAtual -1)*10], BG_BLUE, "");
                    }
                }
            }
        }else if(k == KC_RIGHT){
            if(selected == quantOptions[paginaAtual - 1] + 2){
                // mudar pagina para a direita
                if(paginaAtual < paginas){
                    paginaAtual++;
                    selected = 2 + quantOptions[paginaAtual - 1];
                    printarBufferMateriais(buffer, paginas, paginaAtual, selected, quantOptions);
                }
            }else if(selected > 1){
                char nome[50];
                int quantidade;
                int requisicoes;
                if (sscanf(buffer[selected - 2 + (paginaAtual -1)*10], "%50[^<] < %d > - Requisicoes: %d", nome, &quantidade, &requisicoes) == 3){
                    int len = strlen(nome);
                    nome[len-1] = '\0';
                    if(quantidade < 100){
                        quantidade++;
                        snprintf(buffer[selected - 2 + (paginaAtual -1)*10], 100, "%s < %d > - Requisicoes: %d", nome, quantidade, requisicoes);
                        updateOption(selected + 6, buffer[selected - 2 + (paginaAtual -1)*10], BG_BLUE, "");
                    }
                }
            }
        }else if(k == KC_ENTER){
            if(selected == 1){
                break;
            }
        }
    }

    for(int i = 0; i < paginas; i++){
        for(int j = 0; j < quantOptions[i]; j++){
            char nome[51] = {0};
            int quantidade;
            int requisicoes;
            if (sscanf(buffer[j + i*10], "%51[^<] < %d > - Requisicoes: %d", nome, &quantidade, &requisicoes) == 3){
                int len = strlen(nome);
                nome[len-1] = '\0'; // remove espaço no final
                atualizarQuantidadeEstoque(nome, quantidade);
            }
        }
    } 
    verificarSuspensos();
    // liberar ponteiros
    free(quantOptions);
    for(int i = 0; i < paginas; i++){
        free(buffer[i]);
    }
    free(buffer);
    menuHandler* temp = (menuHandler*)popPilha(estruturasGlobais.pil);
    free(temp);
}