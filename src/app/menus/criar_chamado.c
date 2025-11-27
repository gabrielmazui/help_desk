#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "all.h"

static void limparItems(void* a){
    free(a);
}

static void menuExtraItemEstoque(void){
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
}


static void bufferRecursivo(char ***buffer, arvNo*Atual, int* index){
    if(Atual == NULL) return;
    char nome[50];
    if(*index > 0){
        *buffer = realloc(*buffer, ((*index) + 1) * sizeof(char*));
        (*buffer)[*index] = malloc(60 * sizeof(char));
    }
    strcpy(nome, ((Item*)(Atual->dado))->nome);
    snprintf((*buffer)[*index],60, "%s < 0 >", nome);
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
    (*buffer)[index] = malloc(60 * sizeof(char));
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
    printf(" %sEscolha os materiais necessarios"RESET, GREEN);
    printf("\n\n");
    char* options[2] = {"Material necessario nao esta na lista", "Concluir"};
    for(int i = 0; i < 2; i++){
        if(selected == i + 1){
            printf(" %s%s"RESET, BG_BLUE, options[i]);
        }else{
            printf(" %s%s"RESET, BLUE, options[i]);
        }
        printf("\n");
    }
    printf("\n");
    repetirChar(43, '-', BLUE);
    // 7 linhas de header por pagina
    printf("\n"); // + 2 linhas de espaço
    printf(RED"Escolha no maximo 5 tipos de materiais diferentes"RESET);
    printf("\n\n");

    for(int i = 0; i < quantOptions[paginaAtual - 1]; i++){
        if(selected == i + 3){
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
    if(selected ==  3 + quantOptions[paginaAtual - 1]){
        printf(" %s< Pagina %d/%d >"RESET, BG_BLUE, paginaAtual, paginas);
    }else{
        printf(" %s< Pagina %d/%d >"RESET, BLUE, paginaAtual, paginas);
    }
    fflush(stdout);
}

/// @brief Cria um struct com os dados do tempo atual
/// @return Um struct Tempo com os dados do tempo atual
static Tempo tempoAgora(void){
    Tempo t;

    time_t agora = time(NULL);           // pega o tempo atual em segundos
    struct tm *info = localtime(&agora); // converte para data/hora local

    // copia para a struct personalizada
    t.segundos = info->tm_sec;
    t.minutos  = info->tm_min;
    t.horas    = info->tm_hour;
    t.dia      = info->tm_mday;
    t.mes      = info->tm_mon + 1;     // +1 porque tm_mon vai de 0 a 11
    t.ano      = info->tm_year + 1900; // +1900 porque tm_year conta desde 1900
    return t;
}

/// @brief Imprime o menu para criar um chamado
/// @param type O tipo de pergunta a ser exibida
/// @param entradaErrada Indica se a entrada anterior foi inválida
static void criarChamadoPrintMenu(int type){
    char* pergunta1 = "Qual a prioridade desse chamado?";
    char* pergunta2 = "Qual o titulo do chamado?";
    char* pergunta3 = "Descreva melhor o seu problema:";
    char* options[5] = {"Muito baixa", "Baixa", "Media", "Alta", "Urgente"};
    switch(type){
        case 1:
            repetirChar(15, '-', BLUE);
            printf(BLUE"%s"RESET, "Criar chamado");
            repetirChar(15, '-', BLUE);
            printf("\n");
            printf(GREEN" %s"RESET, pergunta1);
            printf("\n\n");
            for(int i = 0; i < 5; i++){
                if(i == 0)
                    printf(" %s%s"RESET, BG_BLUE, options[i]);
                else{
                    printf(" %s%s"RESET, BLUE, options[i]);

                }
                printf("\n");
            }
            fflush(stdout);
            break;
        case 2:
            repetirChar(15, '-', BLUE);
            printf(BLUE"%s"RESET, "Criar chamado");
            repetirChar(15, '-', BLUE);
            printf("\n");
            
            printf(GREEN" %s"RESET, pergunta2);
            printf("\n\n ");
            fflush(stdout);
            break;
        case 3:
            repetirChar(15, '-', BLUE);
            printf(BLUE"%s"RESET, "Criar chamado");
            repetirChar(15, '-', BLUE);
            printf("\n");
            
            printf(GREEN" %s"RESET, pergunta3);
            printf("\n\n ");
            fflush(stdout);
            break;
    }
}

/// @brief Função para o menu de criar um chamado
/// @param prioridade A prioridade do chamado
/// @param comPreferencia Indica se o chamado tem preferência (1=sim, 2=não)
/// @param usuario O usuário que está criando o chamado
/// @param titulo O título do chamado
/// @param descricao A descrição do chamado
/// @param tipo Qual etapa do menu está sendo executada
void menuCriarChamadoHandler(int prioridade, int comPreferencia, char titulo[50], char descricao[350], int tipo){
    // descobre a prioridade , caso tiver
    // tipo 0 -> perguntar prioridade
    // tipo 1 -> perguntar titulo
    // tipo 2 -> perguntar descricao

    if(comPreferencia == 1 && tipo == 0){
        esperar_tamanho_minimo(17, 45);
        clear();
        criarChamadoPrintMenu(1);
        char* options[5] = {"Muito baixa", "Baixa", "Media", "Alta", "Urgente"};
        int selected = 1;
        while(1){
            KeyCode k = userGetKey();
            if(k == RESIZE_EVENT){
                esperar_tamanho_minimo(17, 45);
                if(terminalPequenoAlertado){
                    terminalPequenoAlertado = 0;
                    clear();
                    criarChamadoPrintMenu(1);
                }
                continue;
            }
            if(k == KC_ESC){
                // se tiver esc volta pro menu de pergunta se tem prioridade
                menuHandler* temp = (menuHandler*)popPilha(estruturasGlobais.pil); // tira o menu de atender chamado
                free(temp); // tira o menu de atender chamado
                clear();
                return;
            }
            else if(k == KC_UP){
                if(selected > 1){
                    updateOption(selected + 3, options[selected - 1], "", BLUE);
                    selected--;
                    updateOption(selected + 3, options[selected - 1], BG_BLUE, "");
                }
            }
            else if(k == KC_DOWN){
                if(selected < 5){
                    updateOption(selected + 3, options[selected - 1], "", BLUE);
                    selected++;
                    updateOption(selected + 3, options[selected - 1], BG_BLUE, "");
                }
            }
            else if(k == KC_ENTER){
                prioridade = selected;
                clear();
                break;
            }
        }
        tipo = 1; // ir para a proxima parte
    }
    if(tipo == 0 && comPreferencia == 2){
        prioridade = 0; // sem preferencia
        tipo = 1; // ir para a proxima parte
    }
    int mostrarBuffer = 1;
    if(tipo == 1){
        // parte 2
        // pergunta o titulo da chamada
        esperar_tamanho_minimo(10, 55);
        clear();
        criarChamadoPrintMenu(2);
        
        while(1){
            KeyCode k2 = inputASCII(titulo, 40, WHITE , mostrarBuffer); // ler input
            // funcao para verificar se e uma string valida
            if(k2 == RESIZE_EVENT){
                esperar_tamanho_minimo(10, 55);
                if(terminalPequenoAlertado){
                    terminalPequenoAlertado = 0;
                    clear();
                    criarChamadoPrintMenu(2);
                    mostrarBuffer = 1;
                    continue;
                }
                mostrarBuffer = 0;
                continue;
            }else if(k2 == KC_ESC){
                // se for pressionado o esc retorna para o menu anterior
                clear();
                if(comPreferencia == 1){
                    // preferencia 0 para permitir que seja perguntado de novo
                    menuCriarChamadoHandler(prioridade, comPreferencia, titulo, descricao, 0);
                }else{
                    menuHandler* temp = (menuHandler*)popPilha(estruturasGlobais.pil); // tira o menu de atender chamado
                    free(temp); // tira o menu de atender chamado
                    // caso nao tiver preferencia, volta para o menu de criar chamado
                }
                // chamo a funcao novamente, para voltar ao menu de qual a preferencia
                // caso apertar o esc denovo, ele voltar para o menu de criar chamado original
                return;
            }
            if(strlen(titulo) == 0){
                continue;
            }
            break;
        }
        clear();
        tipo = 2; // ir para a proxima parte
    }

    if(tipo == 2){
        // parte 3
        // descrever o problema
        esperar_tamanho_minimo(13, 55);
        clear();
        criarChamadoPrintMenu(3);
        mostrarBuffer = 1;
        while(1){
            KeyCode k3 = inputASCII(descricao, 300, WHITE, mostrarBuffer); // ler input

            if(k3 == RESIZE_EVENT){
                esperar_tamanho_minimo(13, 55);
                if(terminalPequenoAlertado){
                    terminalPequenoAlertado = 0;
                    clear();
                    criarChamadoPrintMenu(3);
                    mostrarBuffer = 1;
                    continue;
                }
                mostrarBuffer = 0;
                continue;
            }else if(k3 == KC_ESC){
                // se for pressionado o esc retorna para o primeiro menu
                clear();
                menuCriarChamadoHandler(prioridade, comPreferencia, titulo, descricao, 1);
                return;
            }
            if(strlen(descricao) == 0){
                // descricao invalida
                continue;
            }
            break;
        }
        clear();
    }
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
    char* options[2] = {"Material necessario nao esta na lista", "Concluir"};
    int quantMateriaisSelecionados = 0;
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
            menuCriarChamadoHandler(prioridade, comPreferencia, titulo, descricao, 2);
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
                    snprintf(txt, sizeof(txt), "< Pagina %d/%d >", paginaAtual, paginas);
                    updateOption(25, txt, "", BLUE);
                    selected--;
                    updateOption(5, options[1], BG_BLUE, "");
                }
            }else{
                if(selected == quantOptions[paginaAtual - 1] + 3){
                    char txt[100];
                    snprintf(txt, sizeof(txt), "< Pagina %d/%d >", paginaAtual, paginas);
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
            }else if(selected == 2){
                if(quantOptions[paginaAtual - 1] > 0){

                    updateOption(5, options[1], "", BLUE);
                    selected++; // desce
                    // Coloca destaque na nova opção
                    updateOption(selected + 7, buffer[(selected - 3) + (paginaAtual-1)*10], BG_BLUE, "");
                }else{
                    updateOption(5, options[1], "", BLUE);
                    selected++;
                    char txt[100];
                    snprintf(txt, sizeof(txt), "< Pagina %d/%d >", paginaAtual, paginas);
                    updateOption(25, txt, BG_BLUE, "");
                }
            }else{
                if(selected == quantOptions[paginaAtual - 1] + 2){
                    updateOption(selected + 7, buffer[(selected - 3) + (paginaAtual-1)*10], "", BLUE);
                    selected++;
                    char txt[100];
                    snprintf(txt, sizeof(txt), "< Pagina %d/%d >", paginaAtual, paginas);
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
                    selected = 3+ quantOptions[paginaAtual - 1];
                    printarBufferMateriais(buffer, paginas, paginaAtual, selected, quantOptions);
                }
            }else if(selected > 2){
                char nome[50];
                int quantidade;
                if (sscanf(buffer[selected - 3 + (paginaAtual -1)*10], "%50[^<] < %d >", nome, &quantidade) == 2){
                    int len = strlen(nome);
                    nome[len-1] = '\0';
                    if(quantidade > 0){
                        if(quantidade == 1){
                            quantMateriaisSelecionados--;
                        }
                        quantidade--;
                        snprintf(buffer[selected - 3 + (paginaAtual -1)*10], 60, "%s < %d >", nome, quantidade);
                        updateOption(selected + 7, buffer[selected - 3 + (paginaAtual -1)*10], BG_BLUE, "");
                    }
                }
            }
        }else if(k == KC_RIGHT){
            if(selected == quantOptions[paginaAtual - 1] + 3){
                // mudar pagina para a direita
                if(paginaAtual < paginas){
                    paginaAtual++;
                    selected = 3 + quantOptions[paginaAtual - 1];
                    printarBufferMateriais(buffer, paginas, paginaAtual, selected, quantOptions);
                }
            }else if(selected > 2){
                char nome[50];
                int quantidade;
                if (sscanf(buffer[selected - 3 + (paginaAtual -1)*10], "%50[^<] < %d >", nome, &quantidade) == 2){
                    int len = strlen(nome);
                    nome[len-1] = '\0';
                    if(quantidade < 10 && (quantMateriaisSelecionados < 5 || quantidade > 0)){
                        if(quantidade == 0){
                            quantMateriaisSelecionados++;
                        }
                        quantidade++;
                        snprintf(buffer[selected - 3 + (paginaAtual -1)*10], 60, "%s < %d >", nome, quantidade);
                        updateOption(selected + 7, buffer[selected - 3 + (paginaAtual -1)*10], BG_BLUE, "");
                    }
                }
            }
        }else if(k == KC_ENTER){
            if(selected == 1){
                menuExtraItemEstoque();
                free(quantOptions);
                for(int i = 0; i < paginas; i++){
                    free(buffer[i]);
                }
                free(buffer);
                free(quantOptions);
                buffer = NULL;
                paginas = 0;
                quantOptions = NULL;
                quantMateriaisSelecionados = 0;
                carregarBufferMateriais(&buffer, &paginas, &quantOptions);
                printarBufferMateriais(buffer, paginas, paginaAtual, selected, quantOptions);
                selected = 1;
                continue;
            }else if(selected == 2){
                // concluir
                break;
            }
        }
    }

  
    chamado* cham = malloc(sizeof(chamado));
    cham->id = chamadoIdCounter++;
    strncpy(cham->titulo, titulo, sizeof(cham->titulo)-1);
    cham->titulo[sizeof(cham->titulo)-1] = '\0'; // garante terminação

    strncpy(cham->descricao, descricao, sizeof(cham->descricao)-1);
    cham->descricao[sizeof(cham->descricao)-1] = '\0';

    strncpy(cham->criador, usuario->usuario, sizeof(cham->criador)-1);
    cham->criador[sizeof(cham->criador)-1] = '\0';
    cham->atendente[0] = '\0'; // nenhum atendente ainda

    Tempo t = tempoAgora();
    time_t agora = time(NULL);          // pega o tempo atual em segundos
    cham->tempoSimples = agora;
    cham->tempoComplexo = t;
    cham->materiais = criarFila(limparItems);
    if(comPreferencia == 1){
        cham->prioridade = prioridade;
    }else{
        cham->prioridade = 0;
    }
    cham->cancelado = 0;
    cham->quantMateriais = 0;
    cham->quantMateriaisPorItem = malloc(sizeof(int));
    for(int i = 0; i < paginas; i++){
        for(int j = 0; j < quantOptions[i]; j++){
            char nome[51] = {0};
            int quantidade;
            if (sscanf(buffer[j + i*10], "%51[^<]< %d >", nome, &quantidade) == 2){
                if(quantidade > 0){
                    Item itemTemp = {0};
                    int len = strlen(nome);
                    while (len > 0 && (nome[len-1] == ' ' || nome[len-1] == '\t'))
                        nome[--len] = '\0';
                    strcpy(itemTemp.nome, nome);
                    Item* itemEncontrado = (Item*)arv_buscar(estruturasGlobais.estoque, &itemTemp);
                    if(itemEncontrado == NULL) printf("vazio\n");
                    filaInserir(cham->materiais, itemEncontrado);
                    if(cham->quantMateriais == 0){
                        cham->quantMateriaisPorItem[0] = quantidade;
                    }else{
                        cham->quantMateriaisPorItem = realloc(cham->quantMateriaisPorItem, sizeof(int) * (cham->quantMateriais + 1));
                        cham->quantMateriaisPorItem[cham->quantMateriais] = quantidade;
                    }
                    (cham->quantMateriais)++;
                }
            }
        }
    } 

    
    // liberar ponteiros
    free(quantOptions);
    for(int i = 0; i < paginas; i++){
        free(buffer[i]);
    }
    free(buffer);

    menuHandler* temp = (menuHandler*)popPilha(estruturasGlobais.pil); // tira o menu de atender chamado
    free(temp); // tira o menu de atender chamado
    menuHandler* temp2 = (menuHandler*)popPilha(estruturasGlobais.pil); // tira o menu de atender chamado
    free(temp2); // tira o menu de atender chamado
    criarChamado(cham);
}