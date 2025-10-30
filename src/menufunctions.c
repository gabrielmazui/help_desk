#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "menu.h"
#include "pilha.h"
#include "fila.h"
#include "filaprioridade.h"
#include "utils.h"
#include "menufunctions.h"
#include "colors.h"
#include "filadupla.h"

// funcao para criar um struct com os dados do tempo de agora
static Tempo tempoAgora(void){
    Tempo t;

    time_t agora = time(NULL);           // pega o tempo atual em segundos
    struct tm *info = localtime(&agora); // converte para data/hora local

    // copia para a struct personalizada
    t.segundos = info->tm_sec;
    t.minutos = info->tm_min;
    t.horas   = info->tm_hour;
    t.dia    = info->tm_mday;
    t.mes    = info->tm_mon + 1;     // +1 porque tm_mon vai de 0 a 11
    t.ano    = info->tm_year + 1900; // +1900 porque tm_year conta desde 1900
    return t;
}

// funcao para o menu de criar chamado (preferencia e tiulo do chamado)
static void menuCriarChamadoHandler(int prioridadeParametro, int comPreferencia, Estruturas dados, char* user){
    // menu tipo 3

    // 1 = Sim
    // 2 = nao

    // caso preferencia for maior que 0, significa que ja foi escolhido
    // entao pode ir para o proximo passo

    char tituloChamada[50]; // buffer para o titulo da chamada
    int prioridade = prioridadeParametro; // padrao 0 
    
    char* pergunta = "Qual a prioridade desse chamado?";
    int entradaErrada = 0;
    // descobre a prioridade , caso tiver
    if(comPreferencia == 1 && prioridade == 0){
        while(1){
            printf("\n");
            repetirChar(15, '-', BLUE);
            printf(BLUE"%s"RESET, "Criar chamado");
            repetirChar(15, '-', BLUE);
            printf("\n");
            if(entradaErrada){
                printf(RED" Escolha um numero maior que 0"RESET);
                printf("\n");
            }
            printf(GREEN" %s"RESET, pergunta);
            printf("\n\n ");
            fflush(stdout);
            
            char digitos[7];
            int escVerification = 0;
            // le o input de numero
            inputNumeroASCII(digitos, sizeof(digitos) - 1, WHITE, &escVerification);
            
            // se tiver esc volta pro menu de pergunta se tem prioridade
            if(escVerification){
                popPilha(dados.pil);
                clear();
                return;
            }
            // transforma o vetor de char de digitos em numero    
            for (int i = 0; digitos[i] != '\0'; i++) {
                char c = digitos[i];
                if (c >= '0' && c <= '9') {
                    prioridade = prioridade * 10 + (c - '0'); // constrói o número
                }
            }
            if(prioridade <= 0){
                // continua o loop
                clear();
                entradaErrada = 1;
            }else{
                clear();
                break;
            }
            clear();
        }
    }


    char* pergunta2 = "Qual o titulo do chamado?";
    // parte 2
    // pergunta o titulo da chamada
    printf("\n");
    repetirChar(15, '-', BLUE);
    printf(BLUE"%s"RESET, "Criar chamado");
    repetirChar(15, '-', BLUE);
    printf("\n");
    
    printf(GREEN" %s"RESET, pergunta2);
    printf("\n\n ");
    fflush(stdout);
    
    int escVerification = 0; // variavel para verificar se o ESC foi indentificado no inputASCII
    inputASCII(tituloChamada, 40, WHITE, &escVerification); // ler input
    // funcao para verificar se e uma string valida
    if(escVerification){
        // se for pressionado o esc retorna para o primeiro menu
        clear();
        if(comPreferencia){
            // preferencia 0 para permitir que seja perguntado de novo
            menuCriarChamadoHandler(0, comPreferencia, dados, user);
        }else{
            popPilha(dados.pil);
            // caso nao tiver preferencia, volta para o menu de criar chamado
        }
        // chamo a funcao novamente, para voltar ao menu de qual a preferencia
        // caso apertar o esc denovo, ele voltar para o menu de criar chamado original
        return;
    }
    clear();
    
    // descrever o problema
    char* pergunta3 = "Descreva melhor o seu problema:";
    printf("\n");
    repetirChar(15, '-', BLUE);
    printf(BLUE"%s"RESET, "Criar chamado");
    repetirChar(15, '-', BLUE);
    printf("\n");
    
    printf(GREEN" %s"RESET, pergunta3);
    printf("\n\n ");
    fflush(stdout);
    char descricaoChamada[550]; // buffer para a descricao da chamada
    escVerification = 0; // variavel para verificar se o ESC foi indentificado no inputASCII
    inputASCII(descricaoChamada, 550, WHITE, &escVerification); // ler input
    // funcao para verificar se e uma string valida

    if(escVerification){
        clear();
        menuCriarChamadoHandler(prioridade, comPreferencia, dados, user);
        return;
    }
    clear();


    chamado cham;
    cham.status = 1;
    
    strncpy(cham.titulo, tituloChamada, sizeof(cham.titulo)-1);
    cham.titulo[sizeof(cham.titulo)-1] = '\0'; // garante terminação

    strncpy(cham.descricao, descricaoChamada, sizeof(cham.descricao)-1);
    cham.descricao[sizeof(cham.descricao)-1] = '\0';

    strncpy(cham.criador, user, sizeof(cham.criador)-1);
    cham.criador[sizeof(cham.criador)-1] = '\0';

    Tempo t = tempoAgora();
    time_t agora = time(NULL);          // pega o tempo atual em segundos
    cham.tempoSimples = agora;
    cham.tempoComplexo = t;
    
    if(comPreferencia == 1){
        popPilha(dados.pil);
        popPilha(dados.pil);
        // realizo o pop duas vezes, para voltar direto para o menu principal
        // adiciona na fila de prioridade

        //adiciona na fila de prioridade
        filaPrioridade* f = dados.filaPrioridade;
        cham.prioridade = prioridade;
        filaPrioridadeInserir(f, cham);
    }else{
        fila* f = dados.filaNormal;
        cham.prioridade = 0;
        filaInserir(f, cham);
        popPilha(dados.pil);
        popPilha(dados.pil);
        // realizo o pop duas vezes, para voltar direto para o menu principal
        // adiciona na fila normal
        // tira o esse menu da pilha
    }
}

// visualizar chamados
static void menuEditHandlerChamados(int opcao, Estruturas dados, char* user){
    // menu tipo 5
    // opcao 1 = Atender proximo chamado
    // opcao 2 = Concluir chamado em andamento
    // opcao 3 = Deletar chamado fechado

}

// funcao para menu usuarios
static void menuEditHandlerUsuarios(int userAdm, Estruturas dados, char* user){
    // se e adm ou nao (0 ou 1)
}

static void funcaoMainExtra(int type, int opcao, Estruturas dados, char* user){
    switch(type){
        case 3:
            // criar chamado
            menuCriarChamadoHandler(0, opcao, dados, user);
            break;
        case 5:
            // editar chamados
            break;
        case 7:
            // editar usuarios
            break;
    }
}


// --------------------------------
// FUNCOES DE CONTEUDO EXTRA
// --------------------------------

// funcao auxiliar para mostrar os chamados em baixo
static void mostrarChamados(Estruturas dados) {
    fila *f = dados.filaNormal;
    filaPrioridade *fp = dados.filaPrioridade;
    filaDupla *fd = dados.filadupla;

    noFila *atual1 = f ? f->first : NULL;
    noDuplo *atual2 = fd ? fd->inicio : NULL;

    int fQuant = f ? f->n : 0;
    int fdQuant = fd ? fd->n : 0;
    // criar nova fila normal para em andamento
    int fpQuant = fp ? fp->n : 0;

    int fUsed = 0, fdUsed = 0, fpUsed = 0;
    printf(CYAN" +--------------------------------------------------+     +--------------------------------------------------+     +--------------------------------------------------+\n"RESET);
    printf(CYAN" |                    ABERTOS                       |     |                  EM ANDAMENTO                    |     |                    FECHADOS                      |\n"RESET);
    printf(CYAN" +--------------------------------------------------+     +--------------------------------------------------+     +--------------------------------------------------+\n"RESET);

    // Enquanto houver pelo menos 1 chamado em qualquer coluna
    while (fUsed < fQuant || fdUsed < fdQuant || fpUsed < fpQuant) {

        char buf1[51] = "", buf2[51] = "", buf3[51] = "";

        // Linha 1: Título
        if (fUsed < fQuant && atual1) snprintf(buf1, 51, "%s", atual1->chamado.titulo);
        if (fdUsed < fdQuant && atual2) snprintf(buf2, 51, "%s", atual2->chamado.titulo);
        if (fpUsed < fpQuant) snprintf(buf3, 51, "%s", fp->elementos[fpUsed].titulo);

        if (buf1[0] || buf2[0] || buf3[0])
            printf("| %-50s |   | %-50s |   | %-50s |\n", buf1, buf2, buf3);

        // Linha 2: Prioridade
        if (fUsed < fQuant && atual1) snprintf(buf1, 51, "Prioridade: %d", atual1->chamado.prioridade);
        else buf1[0] = '\0';
        if (fdUsed < fdQuant && atual2) snprintf(buf2, 51, "Prioridade: %d", atual2->chamado.prioridade);
        else buf2[0] = '\0';
        if (fpUsed < fpQuant) snprintf(buf3, 51, "Prioridade: %d", fp->elementos[fpUsed].prioridade);
        else buf3[0] = '\0';

        if (buf1[0] || buf2[0] || buf3[0])
            printf("| %-50s |   | %-50s |   | %-50s |\n", buf1, buf2, buf3);

        // Linha 3: Criador
        if (fUsed < fQuant && atual1) snprintf(buf1, 51, "Criador: %s", atual1->chamado.criador); else buf1[0] = '\0';
        if (fdUsed < fdQuant && atual2) snprintf(buf2, 51, "Criador: %s", atual2->chamado.criador); else buf2[0] = '\0';
        if (fpUsed < fpQuant) snprintf(buf3, 51, "Criador: %s", fp->elementos[fpUsed].criador); else buf3[0] = '\0';

        if (buf1[0] || buf2[0] || buf3[0])
            printf("| %-50s |   | %-50s |   | %-50s |\n", buf1, buf2, buf3);

        // Linha 4: Data / horário
        if (fUsed < fQuant && atual1) snprintf(buf1, 51, "Criado em: %02d/%02d/%04d %02d:%02d:%02d",
                atual1->chamado.tempoComplexo.dia,
                atual1->chamado.tempoComplexo.mes,
                atual1->chamado.tempoComplexo.ano,
                atual1->chamado.tempoComplexo.horas,
                atual1->chamado.tempoComplexo.minutos,
                atual1->chamado.tempoComplexo.segundos);
        else buf1[0] = '\0';
        if (fdUsed < fdQuant && atual2) snprintf(buf2, 51, "Criado em: %02d/%02d/%04d %02d:%02d:%02d",
                atual2->chamado.tempoComplexo.dia,
                atual2->chamado.tempoComplexo.mes,
                atual2->chamado.tempoComplexo.ano,
                atual2->chamado.tempoComplexo.horas,
                atual2->chamado.tempoComplexo.minutos,
                atual2->chamado.tempoComplexo.segundos);
        else buf2[0] = '\0';
        if (fpUsed < fpQuant) snprintf(buf3, 51, "Criado em: %02d/%02d/%04d %02d:%02d:%02d",
                fp->elementos[fpUsed].tempoComplexo.dia,
                fp->elementos[fpUsed].tempoComplexo.mes,
                fp->elementos[fpUsed].tempoComplexo.ano,
                fp->elementos[fpUsed].tempoComplexo.horas,
                fp->elementos[fpUsed].tempoComplexo.minutos,
                fp->elementos[fpUsed].tempoComplexo.segundos);
        else buf3[0] = '\0';

        if (buf1[0] || buf2[0] || buf3[0])
            printf("| %-50s |   | %-50s |   | %-50s |\n", buf1, buf2, buf3);

        // Linha 5: Cabeçalho descrição
        if ((fUsed < fQuant && atual1) || (fdUsed < fdQuant && atual2) || (fpUsed < fpQuant))
            printf("| %-50s |   | %-50s |   | %-50s |\n", "Descricao:", "Descricao:", "Descricao:");

        // Descrição
        int len1 = (fUsed < fQuant && atual1) ? strlen(atual1->chamado.descricao) : 0;
        int len2 = (fdUsed < fdQuant && atual2) ? strlen(atual2->chamado.descricao) : 0;
        int len3 = (fpUsed < fpQuant) ? strlen(fp->elementos[fpUsed].descricao) : 0;

        for (int linha = 0; linha < 8; linha++) {
            int inicio = linha * 50;
            char buffer1[51] = "", buffer2[51] = "", buffer3[51] = "";

            if (fUsed < fQuant && inicio < len1)
                strncpy(buffer1, &atual1->chamado.descricao[inicio], 50);
            if (fdUsed < fdQuant && inicio < len2)
                strncpy(buffer2, &atual2->chamado.descricao[inicio], 50);
            if (fpUsed < fpQuant && inicio < len3)
                strncpy(buffer3, &fp->elementos[fpUsed].descricao[inicio], 50);

            buffer1[50] = buffer2[50] = buffer3[50] = '\0';

            if (buffer1[0] || buffer2[0] || buffer3[0])
                printf("| %-50s |   | %-50s |   | %-50s |\n",
                        buffer1[0] ? buffer1 : "",
                        buffer2[0] ? buffer2 : "",
                        buffer3[0] ? buffer3 : "");
        }

        // Linha de separação
        if ((fUsed < fQuant && atual1) || (fdUsed < fdQuant && atual2) || (fpUsed < fpQuant))
            printf("+------------------------------------------------+   +------------------------------------------------+   +------------------------------------------------+\n");

        // Avança os ponteiros
        if (fUsed < fQuant && atual1) atual1 = atual1->prox;
        if (fdUsed < fdQuant && atual2) atual2 = atual2->prox;
        if (fUsed < fQuant) fUsed++;
        if (fdUsed < fdQuant) fdUsed++;
        if (fpUsed < fpQuant) fpUsed++;

        fflush(stdout);
    }
}



// para estaticas, relatorios e logs, (tipos 6, 8, 9)
// para o tipo 4 tambem (visualizar chamados)
// as info seram printadas em baixo
static void conteudoExtra(int type, Estruturas dados){
    switch(type){
        case 4:
            // visualizar chamados
            mostrarChamados(dados);
            break;
        case 6:
            // estatisticas
            break;
        case 8:
            // relatorios
            break;
        case 9:
            // logs
            break;
    }
}


// vai criar um struct do determinado tipo de menu, e vai retornar ele
static menuHandler createMenu(int type){
    // menu 1 sempre estara na pilha, por ser a "raiz" e ter sido criada no menu.c
    // entao nao e necessario que seja criado aqui
    menuHandler menuReturn;
    switch(type){
        case 2:
            // criar chamado tem preferencia?
            menuReturn.type = 2;
            menuReturn.titulo = "Criar chamado";
            menuReturn.frase = "Esse chamado tem prioridade?";
            menuReturn.quant = 4;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";
            menuReturn.options[2] = "Voltar";
            menuReturn.options[3] = "Sair";
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;
    
        case 3:
            // funcao separada, escolher prioridade > 0 se tiver, e o nome do chamado
            menuReturn = (menuHandler){0};
            menuReturn.type = 3;
            menuReturn.titulo = "";
            menuReturn.frase = "";
            menuReturn.funcaoMain = funcaoMainExtra;
            break;
    
        case 4:
            // filtrar chamados 
            menuReturn.type = 4;
            menuReturn.titulo = "Visualizar Chamados";
            menuReturn.frase = "";
            menuReturn.quant = 5;
            menuReturn.options[0] = "Atender proximo chamado";
            menuReturn.options[1] = "Concluir chamado em andamento";
            menuReturn.options[2] = "Deletar chamado fechado";
            menuReturn.options[3] = "Voltar";
            menuReturn.options[4] = "Sair";
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = conteudoExtra;
            break;
    
        case 5:
            // funcao separada, chamados irao aparecer na direita
            // sera possivel editar pelo botao (ou seta para direita e esquerda)
            menuReturn = (menuHandler){0};
            menuReturn.type = 5;
            menuReturn.titulo = "";
            menuReturn.frase = "";
            menuReturn.funcaoMain = funcaoMainExtra;
            break;
    
        case 6:
            // Estatisticas, vai aparecer as info em baixo do menu de opcoes
            menuReturn.type = 6;
            menuReturn.titulo = "Estatisticas";
            menuReturn.frase = "";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Voltar";
            menuReturn.options[1] = "Sair";
            menuReturn.funcaoExtra = conteudoExtra;
            menuReturn.funcaoMain = NULL;
            break;
    
        case 7:
            // usuarios (caso for adm sera possivel editar)
            // infos vao ficar na direita (edicao via botao ou via seta para direita ou esquerda);
            // funcao separada
            menuReturn = (menuHandler){0};
            menuReturn.type = 7;
            menuReturn.titulo = "";
            menuReturn.frase = "";
            menuReturn.funcaoMain = funcaoMainExtra;
            break;
    
        case 8:
            // relatorios
            // infos em baixo 
            menuReturn.type = 8;
            menuReturn.titulo = "Relatorio das ultimas acoes";
            menuReturn.frase = "";
            menuReturn.quant = 3;
            menuReturn.options[0] = "Deletar todo historico de acoes";
            menuReturn.options[1] = "Voltar";
            menuReturn.options[2] = "Sair";
            menuReturn.funcaoExtra = conteudoExtra;
            menuReturn.funcaoMain = NULL;
            break;
    
        case 9:
            // logs do sistema
            // infos em baixo
            menuReturn.type = 9;
            menuReturn.titulo = "Logs do sistema";
            menuReturn.frase = "";
            menuReturn.quant = 3;
            menuReturn.options[0] = "Deletar todo historico de logs";
            menuReturn.options[1] = "Voltar";
            menuReturn.options[2] = "Sair";
            menuReturn.funcaoExtra = conteudoExtra;
            menuReturn.funcaoMain = NULL;
            break;
    
        case 10:
            // menu de Sair
            // nao tem a opcao sair nem de voltar
            menuReturn.type = 10;
            menuReturn.titulo = "Sair";
            menuReturn.frase = "Voce deseja mesmo fechar o programa?";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;
        case 11:
            // menu de deletar todos relatorios
            // nao tem a opcao sair nem de voltar
            menuReturn.type = 11;
            menuReturn.titulo = "Deletar todos relatorios";
            menuReturn.frase = "Voce deseja mesmo deletar todos os relatorios?";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;
        case 12:
            // menu de deletar logs
            // nao tem a opcao sair nem de voltar
            menuReturn.type = 12;
            menuReturn.titulo = "Deletar logs";
            menuReturn.frase = "Voce deseja mesmo deletar todos os logs?";
            menuReturn.quant = 2;
            menuReturn.options[0] = "Sim";
            menuReturn.options[1] = "Nao";
            menuReturn.funcaoMain = NULL;
            menuReturn.funcaoExtra = NULL;
            break;
    }
    return menuReturn;
}

// funcao para esc
void menuHandlerEsc(int type, pilha* p){
    if(type == 1){
        menuHandler mSair = createMenu(10);
        pushPilha(p, mSair);
    }else{
        popPilha(p);
    }
}

// handler para selecionar as opcoes do menu
void handlerMenuSelect(int type, int selected, pilha **p, char* user){
    menuHandler m;
    switch(type){
        case 1:
            switch(selected){
                case 1:
                    // criar chamado
                    m = createMenu(2);
                    pushPilha(*p, m);
                    break;
                case 2:
                    // visualizar chamados
                    m = createMenu(4);
                    pushPilha(*p, m);
                    break;
                case 3:
                    // Estatísticas
                    m = createMenu(6);
                    pushPilha(*p, m);
                    break;
                case 4:
                    // usuarios
                    m = createMenu(7);
                    pushPilha(*p, m);
                    break;
                case 5:
                    // relatorios
                    m = createMenu(8);
                    pushPilha(*p, m);
                    break;
                case 6:
                    // logs
                    m = createMenu(9);
                    pushPilha(*p, m);
                    break;
                case 7:
                    // sair
                    m = createMenu(10);
                    pushPilha(*p, m);
                    break;
            }
            break;
        case 2:
            // criar acao
            switch(selected){
                case 1:
                case 2:
                    // com ou sem preferencia
                    pushPilha(*p, createMenu(3));
                    fflush(stdout);
                    break;
                case 3:
                    // voltar
                    popPilha(*p);
                    break;
                case 4:
                    // sair
                    pushPilha(*p, createMenu(10));
                    break;
                
            }
            break;
        case 4:
            // visualizar as acoes
            switch(selected){
                case 1:
                case 2:
                case 3:
                    // qual tipo de edicao deseja fazer
                    pushPilha(*p, createMenu(5));
                    break;
                case 4:
                    // voltar
                    popPilha(*p);
                    break;
                case 5:
                    // sair
                    pushPilha(*p, createMenu(10));
                    break;
            }
            break;
        case 6:
            // Estatísticas
            switch(selected){
                // voltar e sair 
                case 1:
                    popPilha(*p);
                    break;
                case 2:
                    pushPilha(*p, createMenu(10));
                    break;
            }
            break;
        case 8:
            // relatorios
            switch(selected){
                case 1:
                    // menu de deletar todos relatorios
                    pushPilha(*p, createMenu(11));
                    break;
                case 2: 
                    // voltar
                    popPilha(*p);
                    break;
                case 3:
                    // sair
                    pushPilha(*p, createMenu(10));
                    break;
            }
            break;
        case 9:
            // logs
            switch(selected){
                case 1:
                    // deletar todos os logs
                    pushPilha(*p, createMenu(12));
                    break;
                case 2:
                    //voltar
                    popPilha(*p);
                    break;
                case 3:
                    // sair
                    pushPilha(*p, createMenu(10));
                    break;
            }
            break;
        case 10:
            // menu de sair
            switch(selected){
                case 1:
                    pilhaLiberar(p);
                    // animacao de sair
                    return;
                case 2:
                    // tira o menu sair do topo da pilha
                    popPilha(*p);
                    break;
            }
            break;
        case 11:
            // menu de deletar todos relatorios
            switch(selected){
                case 1:
                    // deletar todos relatorios
                    // implementar funcao de deletar relatorios
                    popPilha(*p); // tira o menu de deletar relatorios
                    popPilha(*p); // tira o menu de relatorios
                    break;
                case 2:
                    // nao deletar, voltar
                    popPilha(*p); // tira o menu de deletar relatorios
                    break;
            }
            break;
        case 12:
            // menu de deletar todos os logs
            switch(selected){
                case 1:
                    // deletar todos os logs
                    // implementar funcao de deletar logs
                    popPilha(*p); // tira o menu de deletar logs
                    popPilha(*p); // tira o menu de logs
                    break;
                case 2:
                    // nao deletar, voltar
                    popPilha(*p); // tira o menu de deletar logs
                    break;
            }
            break;
    }
}