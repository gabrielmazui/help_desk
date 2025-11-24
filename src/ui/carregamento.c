#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "all.h"

// usleep esta deprecado no Windows, entao criamos um define para compatibilidade
#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)        // Sleep do Windows recebe milissegundos
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((ms)*1000) // usleep recebe microssegundos
#endif


// funcao carregamento de barra pos login
void carregamentoAnimacao(void) {
    const int largura = 30; // tamanho da barra
    int pos = 0;            
    int sentido = 1;        

    time_t start = time(NULL);  
    double duracao = 0;

    while (duracao < 4.0) { // duração 4 segundos
        printf("\r[");

        for (int i = 0; i < largura; i++) {
            if (i == pos)
                printf("#");   // brilho principal
            else if (i == pos - 1 || i == pos + 1)
                printf("=");   // sombra do brilho
            else if (i == pos - 2 || i == pos + 2)
                printf("-");   // sombra mais fraca
            else
                printf(" ");   // fundo da barra
        }

        printf("] Carregando...");
        fflush(stdout);

        sleep_ms(40); // 40ms delay

        pos += sentido;
        if (pos >= largura - 1 || pos <= 0)
            sentido *= -1;

        duracao = difftime(time(NULL), start);
    }

    printf("\r[");
    
    fflush(stdout);
    clear();
}
