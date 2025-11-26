#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "all.h"

static User *bstAtendente(void){
    arvNo* atendentes = estruturasGlobais.atendentes->raiz;
    if(atendentes == NULL) {printf("sem antendetes disponiveis\n"); return NULL;}

    User* atendenteEscolhido = NULL;
    int iteracoes = 0;
    // navegar na arvore binaria para escolher um atendente
    // o menor possivel
    while(1){
        if(atendentes->esq == NULL){
            if(atendentes->dir == NULL){
                atendenteEscolhido = (User*)atendentes->dado;
                break;
            }else if(iteracoes > 0){
                // nao e mais a raiz entao pode escolher o da direita ja que o da esquerda ja foi escolhido
                atendentes = atendentes->dir;
            }
        }else{
            atendentes = atendentes->esq;
        }
        iteracoes++;
    }
    return atendenteEscolhido;
}

int verificarEstoqueChamado(chamado* c){
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
            // decrementar quantidade do estoque
            itemEstoque->quantidade -= c->quantMateriaisPorItem[i]; 
        }else{
            itemEstoque->requisicoes += c->quantMateriaisPorItem[i];
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
    int podeAtender = verificarEstoqueChamado(c);
    
    if(podeAtender){
        filaDuplaInserir(estruturasGlobais.chamadosAndamento,0, c);
        adicionarChamadoTXT(c, 3);
        // escolher atendente
        User* atendente = bstAtendente();
        // colocar referencia do chamado na fila do atendente
        filaInserir(atendente->filaChamados, c);
        atendente->Qchamados += 1;
        // reoorganizar a arvore de atendentes
        arv_remover(estruturasGlobais.atendentes, atendente);
        arv_inserir(estruturasGlobais.atendentes, atendente);
    }else{
       adicionarChamadoTXT(c, 5);
       filaDuplaInserir(estruturasGlobais.chamadosSuspensos, 0, c);
    }
}

/// @brief  Função para concluir um chamado
/// @param indice O índice do chamado a ser concluído
void concluirChamado(int indice){
    filaDupla* fAndamento = estruturasGlobais.chamadosAndamento;
    chamado* c = (chamado*)filaDuplaRemover(fAndamento, indice);
    filaInserir(estruturasGlobais.chamadosConcluidos, c);
    adicionarChamadoTXT(c, 4);
    deletarChamadoTXT(c->id, 3);
    User * atendente = (User*)arv_buscar(estruturasGlobais.atendentes, c->atendente);
    atendente->Qchamados -= 1;

    noFila * no = atendente->filaChamados->first;
    int q = 0;
    if(((chamado*)(no->dado))->id == c->id){
        filaRetirar(atendente->filaChamados);
    }
    noFila * noTemp = no->prox;
    while(no != NULL){
        if(((chamado*)noTemp->dado)->id == c->id){
            // remover a referencia do chamado na fila do atendente
            no->prox = noTemp->prox;
            atendente->filaChamados->liberar(noTemp->dado);
            atendente->filaChamados->n -= 1;
            free(noTemp);
            break;
        }
        no = no->prox;
        q++;
    }
    // reoorganizar a arvore de atendentes
    arv_remover(estruturasGlobais.atendentes, atendente);
    arv_inserir(estruturasGlobais.atendentes, atendente);
}

/// @brief deletar um chamado concluido
/// @param indice O índice do chamado a ser deletado
void deletarChamado(int indice, int todos){
    if(todos){
        filaDuplaEsvaziar(estruturasGlobais.chamadosSuspensos);
        deletarTodosChamadosTXT();
    }else{
        filaDupla* fd = estruturasGlobais.chamadosAndamento;
        chamado* c = (chamado*)filaDuplaRemover(fd, indice);
        filaDuplaRemover(fd, indice);
        deletarChamadoTXT(c->id, 4);
    }
}

/// @brief cancelar um chamado
/// @param indice O índice do chamado a ser cancelado
/// @param type tipo do chamado (1 = aberto, 2 = andamento, 3 = suspenso)
void cancelarChamado(int indice, int type){ 
    // indice nao conta para chamados abertos
    switch(type){
        case 1: {
            int prioridade = 0;
            chamado* c = NULL;
            if(estruturasGlobais.chamadosAbertosComPrioridade->n > 0){
                prioridade = 1;
                c = (chamado*)filaPrioridadeRemover(estruturasGlobais.chamadosAbertosComPrioridade);
            }else{
                c = (chamado*)filaRetirar(estruturasGlobais.chamadosAbertosSemPrioridade);
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
            adicionarChamadoTXT(c, 5);
            deletarChamadoTXT(c->id, 3);
            User * atendente = (User*)arv_buscar(estruturasGlobais.atendentes, c->atendente);
            strcpy(c->atendente, ""); // remover atendente do chamado
            atendente->Qchamados -= 1;
            noFila * no = atendente->filaChamados->first;
            int q = 0;
            if(((chamado*)(no->dado))->id == c->id){
                filaRetirar(atendente->filaChamados);
            }
            noFila * noTemp = no->prox;
            while(no != NULL){
                if(((chamado*)noTemp->dado)->id == c->id){
                    // remover a referencia do chamado na fila do atendente
                    no->prox = noTemp->prox;
                    atendente->filaChamados->liberar(noTemp->dado);
                    atendente->filaChamados->n -= 1;
                    free(noTemp);
                    break;
                }
                no = no->prox;
                q++;
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
            filaInserir(estruturasGlobais.chamadosConcluidos, c);
            break;
        }
    }
}

void atenderChamadoSuspenso(int indice){
    filaDupla* fSuspensos = estruturasGlobais.chamadosSuspensos;
    chamado* c = (chamado*)filaDuplaRemover(fSuspensos, indice);
    deletarChamadoTXT(c->id, 5);
    c->prioridade += 6; // colocar maior prioridade possivel para ser atendido primeiro
    // colocar na fila de prioridade
    filaPrioridadeInserir(estruturasGlobais.chamadosAbertosComPrioridade, c);
    adicionarChamadoTXT(c, 1);
}