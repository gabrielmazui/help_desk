#ifndef ESTOQUE_H
#define ESTOQUE_H

typedef struct Item Item;

void carregarEstoqueTXT(void);
void atualizarQuantidadeEstoque(char* itemNome, int novaQuantidade);
void adicionarItemEstoque(Item* novoItem);
void deletarItemEstoque(char* itemNome);

#endif