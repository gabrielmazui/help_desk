#ifndef PILHA_H
#define PILHA_H

typedef struct pilha{
    int n;  // numero de elemntos 
    int dim; // dimensao maxima do vetor
    void** vet;
}pilha;

// funcao para criar uma pilha (retorna um ponteiro para ela)
pilha* criarPilha(void);
// inserir elemento no topo (push)
void pushPilha(pilha* p, void* dado);
// remover elemento do topo (pop)
void* popPilha(pilha* p);
//mostrar o topo da pilha
void* topoPilha(pilha * p);
// liberar a pilha
void pilhaLiberar(pilha** p);

#endif