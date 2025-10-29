#ifndef PILHA_H
#define PILHA_H

typedef struct menuHandler menuHandler;

typedef struct pilha{
    int n;  // numero de elemntos 
    int dim; // dimensao maxima do vetor
    menuHandler* vet;
}pilha;

// funcao para criar uma pilha (retorna um ponteiro para ela)
pilha* criarPilha(void);
// inserir elemento no topo (push)
void pushPilha(pilha* p, menuHandler menu);
// remover elemento do topo (pop)
menuHandler popPilha(pilha* p);
//mostrar o topo da pilha
menuHandler topoPilha(pilha * p);
// liberar a pilha
void pilhaLiberar(pilha** p);

#endif