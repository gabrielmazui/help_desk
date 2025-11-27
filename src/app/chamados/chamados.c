#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "all.h"

static User* bstAtendente(void) {
    arvNo *atendentes = estruturasGlobais.atendentes->raiz;
    if (!atendentes) {
        printf("sem atendentes disponiveis\n");
        return NULL;
    }

    // menor Qchamados = mais à esquerda
    while (atendentes->esq != NULL) {
        atendentes = atendentes->esq;
    }

    return (User*)atendentes->dado;
}

/// @brief função para verificar se o chamado pode ser atendido com o estoque atual
/// @param c chamado a ser verificado
/// @param tipo tipo do chamado (1 = abertos, 2 = suspenso)
/// @return inteiro indicando se pode ser atendido (1) ou nao (0)
static int verificarEstoqueChamado(chamado* c, int tipo){
    // verificar se o chamado tem os itens suficientes no estoque
    arvoreBinaria* estoque = estruturasGlobais.estoque;
    // percorrer a arvore para cada item do chamado
    noFila* noItemChamado = c->materiais->first;
    int podeAtender = 1;
    int i = 0;
    while(noItemChamado != NULL){
        Item* itemChamado = (Item*)noItemChamado->dado;
        Item* itemEstoque = (Item*)arv_buscar(estoque, itemChamado);
        // buscar o item na arvore
        
        if(itemEstoque == NULL){
            // item nao encontrado
            podeAtender = 0;
            Item* item = malloc(sizeof(Item));
            item->id = itemIdCounter++;
            item->quantidade = 0;
            item->requisicoes = 0;
            strcpy(item->nome, itemChamado->nome);
            alterarVariavelConfig(3, itemIdCounter);
            adicionarItemEstoque(item);
            break;
            
        }else{
            // verificar quantidade
            if(itemEstoque->quantidade < c->quantMateriaisPorItem[i]){
                podeAtender = 0;
                break;
            }
        }
        i++;
        noItemChamado = noItemChamado->prox;
    }

    noItemChamado = c->materiais->first;
    i = 0;
    while(noItemChamado != NULL){
        Item* itemChamado = (Item*)noItemChamado->dado;
        Item* itemEstoque = (Item*)arv_buscar(estoque, itemChamado);
        // buscar o item na arvore
        if(podeAtender){
            if(tipo == 1){
                // decrementar quantidade do estoque
                itemEstoque->quantidade -= c->quantMateriaisPorItem[i];
                // decrementar do estoque
                atualizarQuantidadeEstoque(itemEstoque->nome, itemEstoque->quantidade);
            }
            if(tipo == 2){
                itemEstoque->requisicoes -= c->quantMateriaisPorItem[i];
                // decrementar do requisicoes do estoque
                atualizarRequisicoesEstoque(itemEstoque->nome, itemEstoque->requisicoes);
            }
        }else if(tipo == 1){
            itemEstoque->requisicoes += c->quantMateriaisPorItem[i];
            // incrementar requisicoes do estoque
            atualizarRequisicoesEstoque(itemEstoque->nome, itemEstoque->requisicoes);
        }
        i++;
        noItemChamado = noItemChamado->prox;
    }
    return podeAtender;
}

/// @brief Cria um chamado e adiciona na fila correta
/// @param c O chamado a ser criado
void criarChamado(chamado* c){
    // funcao para criar um chamado e adicionar na fila de prioridade
    if(c->prioridade <= 0){
        fila* fNormal = estruturasGlobais.chamadosAbertosSemPrioridade;
        filaInserir(fNormal, c);
    }else{
        filaPrioridade* fp = estruturasGlobais.chamadosAbertosComPrioridade;
        filaPrioridadeInserir(fp, c);
    }
    alterarVariavelConfig(2, chamadoIdCounter);
    adicionarChamadoTXT(c, c->prioridade > 0 ? 1 : 2);
}

/// @brief  Função para atender um chamado
void atenderChamado(void){
    chamado* c = NULL;
    
    if(estruturasGlobais.chamadosAbertosComPrioridade->n > 0)
        c = (chamado*)filaPrioridadeRemover(estruturasGlobais.chamadosAbertosComPrioridade);
    else{
        c = (chamado*)filaRetirar(estruturasGlobais.chamadosAbertosSemPrioridade);
    }
    deletarChamadoTXT(c->id, c->prioridade > 0 ? 1 : 2);
    // verificar se o chamado tem os itens suficientes no estoque
    int podeAtender = verificarEstoqueChamado(c, 1);
    
    if(podeAtender){
        filaDuplaInserir(estruturasGlobais.chamadosAndamento,0, c);
        // escolher atendente
        User* atendente = bstAtendente();
        strcpy(c->atendente, atendente->usuario);
        // colocar referencia do chamado na fila do atendente
        filaInserir(atendente->filaChamados, c);
        atendente->Qchamados += 1;
        c->QchamadosAtendente = &atendente->Qchamados; // ponteiro para a quantidade de chamados do atendente
        // reoorganizar a arvore de atendentes
        arv_remover(estruturasGlobais.atendentes, atendente);
        arv_inserir(estruturasGlobais.atendentes, atendente);
        adicionarChamadoTXT(c, 3);
    }else{
       adicionarChamadoTXT(c, 5);
       filaDuplaInserir(estruturasGlobais.chamadosSuspensos, 0, c);
    }
}

/// @brief  Função para concluir um chamado
/// @param indice O índice do chamado a ser concluído
void concluirChamado(int indice){
    filaDupla* fAndamento = estruturasGlobais.chamadosAndamento;
    int q = 0;
    int contador = 0;
    noDuplo* no = fAndamento->inicio;
    while(no != NULL && q < indice){
        if(strcmp(((chamado*)no->dado)->atendente, usuario->usuario) == 0){
            q++;
        }
        contador++;
        no = no->prox;
    }
    indice = contador;
    chamado* c = (chamado*)filaDuplaRemover(fAndamento, indice);
    filaInserir(estruturasGlobais.chamadosConcluidos, c);
    adicionarChamadoTXT(c, 4);
    deletarChamadoTXT(c->id, 3);
    User temp = {0};
    strcpy(temp.usuario, c->atendente);
    temp.Qchamados = *(c->QchamadosAtendente);
    User *atendente = (User*)arv_buscar(estruturasGlobais.atendentes, &temp);
    atendente->Qchamados -= 1;

    noFila *noAt = atendente->filaChamados->first;
    int idx = 0;
    while (noAt != NULL) {
        chamado *ch = (chamado*)noAt->dado;

        if (ch->id == c->id) {
            filaRetirarIndice(atendente->filaChamados, idx);
            break;
        }
        noAt = noAt->prox;
        idx++;
    }

    // reoorganizar a arvore de atendentes
    arv_remover(estruturasGlobais.atendentes, atendente);
    arv_inserir(estruturasGlobais.atendentes, atendente);
}

/// @brief deletar um chamado concluido
/// @param indice O índice do chamado a ser deletado
/// @param todos Indica se todos os chamados devem ser deletados
void deletarChamado(int indice, int todos){
    if(todos){
        noFila* no = estruturasGlobais.chamadosConcluidos->first;
        while(no != NULL){
            chamado* c = filaRetirar(estruturasGlobais.chamadosConcluidos);
            estruturasGlobais.chamadosConcluidos->liberar(c);
            no = estruturasGlobais.chamadosConcluidos->first;
        }
        deletarTodosChamadosTXT();
    }else{
        fila* f = estruturasGlobais.chamadosConcluidos;
        chamado* c = (chamado*)filaRemoverIndice(f, indice);
        deletarChamadoTXT(c->id, 4);
        estruturasGlobais.chamadosConcluidos->liberar(c);
    }
}

/// @brief cancelar um chamado
/// @param indice O índice do chamado a ser cancelado
/// @param type tipo do chamado (1 = aberto, 2 = andamento, 3 = suspenso)
void cancelarChamado(int indice, int type){ 
    switch(type){
        case 1: {
            int prioridade = 0;
            chamado* c = NULL;
            filaPrioridade* fp = estruturasGlobais.chamadosAbertosComPrioridade;
            
            if(indice < fp->n){
                c = (chamado*)filaPrioridadeRemoverIndice(fp, indice);
                prioridade = 1;
                
            }else{
                int quantChamadosPrioridade = fp->n;
                fila* fNormal = estruturasGlobais.chamadosAbertosSemPrioridade;
                noFila* no = fNormal->first;
                while(no != NULL){
                    if(quantChamadosPrioridade == indice){
                        c = (chamado*)no->dado;
                        filaRemoverIndice(fNormal, indice - fp->n);
                        break;
                    }
                    quantChamadosPrioridade++;
                    no = no->prox;
                }
            }
            c->cancelado = 1;
            adicionarChamadoTXT(c, 4);
            deletarChamadoTXT(c->id, (prioridade) ? 1 : 2);
            break;
        }
        case 2: {
            filaDupla* fAndamento = estruturasGlobais.chamadosAndamento;
            chamado* c = (chamado*)filaDuplaRemover(fAndamento, indice);
            c->cancelado = 1;
            filaInserir(estruturasGlobais.chamadosConcluidos, c);
            adicionarChamadoTXT(c, 4);
            deletarChamadoTXT(c->id, 3);
            User temp = {0};
            strcpy(temp.usuario, c->atendente);
            temp.Qchamados = *(c->QchamadosAtendente);
            User *atendente = (User*)arv_buscar(estruturasGlobais.atendentes, &temp);
            strcpy(c->atendente, ""); // remover atendente do chamado
            atendente->Qchamados -= 1;
            noFila *no = atendente->filaChamados->first;
            int q = 0;
            chamado *cham = NULL;

            while (no != NULL) {

                chamado *cTemp = (chamado*)no->dado;

                if (cTemp->id == c->id) {
                    // remove no índice q
                    cham = (chamado*)filaRemoverIndice(atendente->filaChamados, q);
                    break;
                }

                no = no->prox;
                q++;
            }
            fila * fmat = cham->materiais;
            noFila * noMat = fmat->first;
            for(int i = 0; i < cham->quantMateriais; i++){
                Item* itemChamado = (Item*)noMat->dado;
                Item* itemEstoque = (Item*)arv_buscar(estruturasGlobais.estoque, itemChamado);
                itemEstoque->quantidade += cham->quantMateriaisPorItem[i];
                // incrementar do estoque
                atualizarQuantidadeEstoque(itemEstoque->nome, itemEstoque->quantidade);
                noMat = noMat->prox;
            }
            // reoorganizar a arvore de atendentes
            arv_remover(estruturasGlobais.atendentes, atendente);
            arv_inserir(estruturasGlobais.atendentes, atendente);
            break;
        }
        case 3: {
            filaDupla* fSuspensos = estruturasGlobais.chamadosSuspensos;
            chamado* c = (chamado*)filaDuplaRemover(fSuspensos, indice);
            c->cancelado = 1;
            adicionarChamadoTXT(c, 4);
            deletarChamadoTXT(c->id, 5);
            fila * f = c->materiais;
            noFila * no = f->first;
            for(int i = 0; i < c->quantMateriais; i++){
                Item* itemChamado = (Item*)no->dado;
                Item* itemEstoque = (Item*)arv_buscar(estruturasGlobais.estoque, itemChamado);
                itemEstoque->requisicoes -= c->quantMateriaisPorItem[i];
                // decrementar do requisicoes do estoque
                atualizarRequisicoesEstoque(itemEstoque->nome, itemEstoque->requisicoes);
                no = no->prox;
            }
            filaInserir(estruturasGlobais.chamadosConcluidos, c);
            break;
        }
    }
}

void atenderChamadoSuspenso(int indice){
    filaDupla* fSuspensos = estruturasGlobais.chamadosSuspensos;
    chamado* c = (chamado*)filaDuplaRemover(fSuspensos, indice);
    fila * f = c->materiais;
    noFila * no = f->first;
    for(int i = 0; i < c->quantMateriais; i++){
        Item* itemChamado = (Item*)no->dado;
        Item* itemEstoque = (Item*)arv_buscar(estruturasGlobais.estoque, itemChamado);
        // buscar o item na arvore
        // decrementar quantidade do estoque
        itemEstoque->quantidade -= c->quantMateriaisPorItem[i];
        itemEstoque->requisicoes -= c->quantMateriaisPorItem[i];
        // decrementar do estoque
        atualizarQuantidadeEstoque(itemEstoque->nome, itemEstoque->quantidade);
        atualizarRequisicoesEstoque(itemEstoque->nome, itemEstoque->requisicoes);
        no = no->prox;
    }
    deletarChamadoTXT(c->id, 5);
    c->prioridade += 6; // colocar maior prioridade possivel para ser atendido primeiro
    // colocar na fila de prioridade
    filaPrioridadeInserir(estruturasGlobais.chamadosAbertosComPrioridade, c);
    adicionarChamadoTXT(c, 1);
}

/// @brief verifica os chamados suspensos para ver se podem ser atendidos 
void verificarSuspensos(void){
    filaDupla* fSuspensos = estruturasGlobais.chamadosSuspensos;
    noDuplo* no = fSuspensos->inicio;
    int indice = 0;
    while(no != NULL){
        chamado* c = (chamado*)no->dado;
        int podeAtender = verificarEstoqueChamado(c, 2);
        no = no->prox; // avancar antes de possivel remoção
        if(podeAtender){
            // remover da fila de suspensos
            filaDuplaRemover(fSuspensos, indice);
            deletarChamadoTXT(c->id, 5);
            c->prioridade += 6; // colocar maior prioridade possivel para ser atendido primeiro
            // colocar na fila de prioridade
            filaPrioridadeInserir(estruturasGlobais.chamadosAbertosComPrioridade, c);
            adicionarChamadoTXT(c, 1);
        }else{
            indice++;
        }
    }
}

static arvNo* recursaoIndiceEstoque(arvNo* no, int* contador, int indice){
    if(!no) return NULL;

    arvNo* r = recursaoIndiceEstoque(no->esq, contador, indice);
    if(r) return r;

    Item* it = (Item*)no->dado;
    if(it->quantidade == 0 && it->requisicoes == 0){
        if(*contador == indice){
            return no;
        }
        (*contador)++;
    }
    return recursaoIndiceEstoque(no->dir, contador, indice);
}


void deletarItemChamadoEstoque(int indice){
    arvoreBinaria* estoque = estruturasGlobais.estoque;

    int contador = 0;
    arvNo* no = recursaoIndiceEstoque(estoque->raiz, &contador, indice);

    if(!no){
        printf("Item %d não encontrado para remoção.\n", indice);
        return;
    }

    Item *it = (Item*)no->dado;
    deletarItemEstoque(it->nome);
}

static int existeItemDeletavelRec(arvNo *no) {
    if(!no) return 0;

    if(existeItemDeletavelRec(no->esq)) return 1;

    Item *it = (Item*)no->dado;
    if(it->quantidade == 0 && it->requisicoes == 0)
        return 1;

    return existeItemDeletavelRec(no->dir);
}

int existeItemDeletavel(arvoreBinaria *arv) {
    return existeItemDeletavelRec(arv->raiz);
}

