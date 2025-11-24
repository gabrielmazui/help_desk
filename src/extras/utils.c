#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifndef _WIN32
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#else
#include <conio.h>
#endif

#include "all.h"

#ifdef _WIN32
#include <windows.h> // apenas no Windows
#endif


void controlarCursor(int visible) {
    // controla visibilidade do cursor de texto windows e linux/macOS
    #ifdef _WIN32
        CONSOLE_CURSOR_INFO info;
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleCursorInfo(h, &info);
        info.bVisible = visible;
        SetConsoleCursorInfo(h, &info);
    #else
        if(visible) {
            printf("\033[?25h"); // mostra cursor no Linux/macOS
        } else {
            printf("\033[?25l"); // esconde cursor no Linux/macOS
        }
        fflush(stdout);
    #endif
}

// funcao para repetir chars na tela (para deixar mais simetrico)
void repetirChar(int n, char c, char* COLOR){
    // n -> quantas vezes
    // c -> qual char para repetir
    // COLOR -> cor para printar no terminar (pode ser "" em caso de branco)
    for(int i = 0; i < n; i++){
        printf("%s%c"RESET, COLOR, c);
    }
}

// funcao para comparar 2 strings
int compStr(const char a[], const char b[]){
    // char a , char b -> strings para comparar (constantes pois nao vao ser alteradas) 
    int i = 0;
    while(a[i] != '\0'){
        if(a[i] != b[i]){
            return 0;
        }
        i++;
    }
    return a[i] == b[i];
}

// funcao para windowns e linux/macos conseguirem ler
// char sem enter no terminal
char getChar(void) {
#ifdef _WIN32
    return _getch();
#else
    char c;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    read(STDIN_FILENO, &c, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
#endif
}

// usa o getChar
// funcao para ler as teclas (esc, seta, enter)
// meio complicado a tecla ESC, pois as setas em si comecam com ESC
// o ESC sozinho e 27 na tabela ASCII
// e as setas tambem comecam com isso

/// @brief  Função para ler a tecla pressionada pelo usuário. 
/// @return keycode correspondente à tecla pressionada.

KeyCode userGetKey(void) {
#ifdef _WIN32
    while (1) {
        // Detecta resize primeiro
        if (houve_resize())
            return RESIZE_EVENT;

        // Verifica se há tecla pressionada
        if (_kbhit()) {
            int c = _getch();

            // Teclas especiais (setas)
            if (c == 0 || c == 224) { 
                int arrow = _getch();
                switch (arrow) {
                    case 72: return KC_UP;
                    case 80: return KC_DOWN;
                    case 75: return KC_LEFT;
                    case 77: return KC_RIGHT;
                    default: return KC_OTHER;
                }
            }

            // ESC
            else if (c == 27){
                return KC_ESC;
            }

            // Enter
            else if (c == 13) return KC_ENTER;

            // Outras teclas
            else return KC_OTHER;
        }

        Sleep(1); // loop leve
    }

#else // Linux / macOS
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // modo raw
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    fd_set set;
    struct timeval timeout;
    unsigned char c = 0;

    while (1) {
        // Detecta resize primeiro
        if (houve_resize()) {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return RESIZE_EVENT;
        }

        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000; // 1ms

        int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
        if (rv > 0 && FD_ISSET(STDIN_FILENO, &set)) {
            read(STDIN_FILENO, &c, 1);

            // Setas via sequência ANSI
            if (c == 27) { 
                unsigned char seq[2] = {0,0};
                usleep(5000);
                int bytesWaiting = 0;
                ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
                if (bytesWaiting >= 2) {
                    read(STDIN_FILENO, &seq, 2);
                    if (seq[0] == '[') {
                        switch (seq[1]) {
                            case 'A': tcsetattr(STDIN_FILENO, TCSANOW, &oldt); return KC_UP;
                            case 'B': tcsetattr(STDIN_FILENO, TCSANOW, &oldt); return KC_DOWN;
                            case 'C': tcsetattr(STDIN_FILENO, TCSANOW, &oldt); return KC_RIGHT;
                            case 'D': tcsetattr(STDIN_FILENO, TCSANOW, &oldt); return KC_LEFT;
                        }
                    }
                }
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return KC_ESC;
            }

            // Enter
            else if (c == '\r' || c == '\n') {
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return KC_ENTER;
            }

            // Qualquer outra tecla
            else {
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return KC_OTHER;
            }
        }
        // rv == 0 → timeout, apenas continua o loop
    }
#endif
}

static void limparBufferStdin() {
    // limpa o buffer do stdin
    #ifdef _WIN32
        while (_kbhit()) {
            _getch();
        }
    #else
        tcflush(STDIN_FILENO, TCIFLUSH); // descarta dados de entrada pendentes
    #endif
}

/// @brief Função para ler uma entrada de texto ASCII
/// @param buffer O buffer onde a entrada será armazenada
/// @param maxChars O número máximo de caracteres a serem lidos
/// @param color A cor do texto a ser exibido
/// @param printarExistente Se deve ou não imprimir o texto existente no buffer
/// @return O código da tecla pressionada
KeyCode inputASCII(char *buffer, int maxChars, const char *color, int printarExistente) {
    int qCharsLinhas[7] = {0}; // chars visuais de cada linha
    int linhaStart[7] = {0};   // índice no buffer onde cada linha começa
    int linhaAtual = 0;
    limparBufferStdin();
    controlarCursor(1);
    int bufferLen = strlen(buffer);
    int currentLineCharsVisual = 0; // chars na tela
    char c = 0;

    printf("%s", color);

    // ================== PRINTA TEXTO EXISTENTE ==================
    if (printarExistente && bufferLen > 0) {
        linhaStart[linhaAtual] = 0;
        if(currentLineCharsVisual != 0){
            putchar(' '); // espaço inicial
        }
        currentLineCharsVisual++;

        for (int i = 0; i < bufferLen; i++) {
            putchar(buffer[i]);
            fflush(stdout);
            currentLineCharsVisual++;

            if (currentLineCharsVisual >= 50) {
                // procura o último espaço na linha atual
                int breakPos = -1;
                for (int k = i; k >= linhaStart[linhaAtual]; k--) {
                    if (buffer[k] == ' ') {
                        breakPos = k;
                        break;
                    }
                }
                if (breakPos == -1) breakPos = i;

                int charsRestantes = i - breakPos;

                // atualiza qCharsLinhas antes de mover a palavra
                qCharsLinhas[linhaAtual] = breakPos - linhaStart[linhaAtual] + 1; // +1 espaço inicial

                // apaga palavra cortada
                for (int j = i; j > breakPos; j--) printf("\b \b");

                printf("\n%s", color); // nova linha
                putchar(' ');
                fflush(stdout);

                linhaAtual++;
                if (linhaAtual >= 7) linhaAtual = 6; // segurança
                linhaStart[linhaAtual] = breakPos + 1;
                currentLineCharsVisual = charsRestantes + 1; // +1 espaço inicial

                // imprime os chars restantes
                for (int j = 0; j < charsRestantes; j++)
                    putchar(buffer[linhaStart[linhaAtual] + j]);
                fflush(stdout);

                i = breakPos + charsRestantes; // ajusta índice do loop
            }
        }
    }
    fflush(stdout);

    // ================== LOOP DE ENTRADA ==================
    #ifdef _WIN32
    while (1) {
        if (houve_resize()) return RESIZE_EVENT;
        if (_kbhit()) {
            c = getChar();

            if (c == 27){
                controlarCursor(0);
                printf(RESET);
                return KC_ESC;
            }
            if (c == '\r') break;

            // -------- BACKSPACE ----------
            if (c == 127 || c == '\b') {
                if (bufferLen > 0) {
                    bufferLen--;
                    buffer[bufferLen] = '\0';
                    currentLineCharsVisual--;
                    printf("\b \b");
                    fflush(stdout);

                    if (currentLineCharsVisual <= 0 && linhaAtual > 0) {
                        linhaAtual--;
                        printf("\033[A"); // sobe linha
                        printf("\033[%dG", qCharsLinhas[linhaAtual] + 1); // vai pro final da linha acima (+1 espaço)
                        fflush(stdout);
                        currentLineCharsVisual = qCharsLinhas[linhaAtual];
                    }
                }
            }

            // -------- CARACTERE VISÍVEL ----------
            else if (c >= 32 && c < 127) {
                if (bufferLen < maxChars - 1) {
                    if (c == ' ' && bufferLen == 0) continue;
                    if (c == ' ' && buffer[bufferLen - 1] == ' ') continue;

                    buffer[bufferLen++] = c;
                    buffer[bufferLen] = '\0';
                    putchar(c);
                    fflush(stdout);
                    currentLineCharsVisual++;

                    // quebra de linha automática
                    if (currentLineCharsVisual >= 50) {
                        // procura o último espaço da linha atual
                        int breakPos = -1;
                        for (int k = bufferLen - 1; k >= linhaStart[linhaAtual]; k--) {
                            if (buffer[k] == ' ') {
                                breakPos = k;
                                break;
                            }
                        }
                        if (breakPos == -1) breakPos = bufferLen - 1;

                        int charsRestantes = bufferLen - (breakPos + 1);

                        // atualiza qCharsLinhas da linha atual
                        qCharsLinhas[linhaAtual] = breakPos - linhaStart[linhaAtual] + 1;

                        // apaga palavra cortada
                        for (int j = bufferLen - 1; j > breakPos; j--) printf("\b \b");

                        printf("\n%s", color);
                        putchar(' ');
                        fflush(stdout);

                        linhaAtual++;
                        if (linhaAtual >= 7) linhaAtual = 6;
                        linhaStart[linhaAtual] = breakPos + 1;
                        currentLineCharsVisual = charsRestantes + 1;

                        for (int j = 0; j < charsRestantes; j++)
                            putchar(buffer[linhaStart[linhaAtual] + j]);
                        fflush(stdout);

                        bufferLen = breakPos + charsRestantes + 1; // ajusta bufferLen
                    }
                }
            }
        }
        Sleep(1);
    }

#else // Linux / macOS
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    fd_set set;
    struct timeval timeout;

    while (1) {
        if (houve_resize()) {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return RESIZE_EVENT;
        }

        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;

        int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
        if (rv > 0 && FD_ISSET(STDIN_FILENO, &set)) {
            read(STDIN_FILENO, &c, 1);

            if (c == 27) { // ESC
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                controlarCursor(0);
                printf(RESET);
                return KC_ESC;
            }
            if (c == '\r' || c == '\n') break;

            // -------- BACKSPACE ----------
            if (c == 127 || c == '\b') {
                if (bufferLen > 0) {
                    bufferLen--;
                    buffer[bufferLen] = '\0';
                    currentLineCharsVisual--;
                    printf("\b \b");
                    fflush(stdout);

                    if (currentLineCharsVisual <= 0 && linhaAtual > 0) {
                        linhaAtual--;
                        printf("\033[A"); // sobe linha
                        printf("\033[%dG", qCharsLinhas[linhaAtual] + 1); // final da linha acima
                        fflush(stdout);
                        currentLineCharsVisual = qCharsLinhas[linhaAtual];
                    }
                }
            }

            // -------- CARACTERE VISÍVEL ----------
            else if (c >= 32 && c < 127) {
                if (bufferLen < maxChars - 1) {
                    if (c == ' ' && bufferLen == 0) continue;
                    if (c == ' ' && buffer[bufferLen - 1] == ' ') continue;

                    buffer[bufferLen++] = c;
                    buffer[bufferLen] = '\0';
                    putchar(c);
                    fflush(stdout);
                    currentLineCharsVisual++;

                    // quebra de linha automática
                    if (currentLineCharsVisual >= 50) {
                        // procura último espaço da linha atual
                        int breakPos = -1;
                        for (int k = bufferLen - 1; k >= linhaStart[linhaAtual]; k--) {
                            if (buffer[k] == ' ') {
                                breakPos = k;
                                break;
                            }
                        }
                        if (breakPos == -1) breakPos = bufferLen - 1;

                        int charsRestantes = bufferLen - (breakPos + 1);

                        // atualiza qCharsLinhas da linha atual
                        qCharsLinhas[linhaAtual] = breakPos - linhaStart[linhaAtual] + 1;

                        // apaga palavra cortada
                        for (int j = bufferLen - 1; j > breakPos; j--) printf("\b \b");

                        printf("\n%s", color);
                        putchar(' '); // espaço inicial da nova linha
                        fflush(stdout);

                        linhaAtual++;
                        if (linhaAtual >= 7) linhaAtual = 6;
                        linhaStart[linhaAtual] = breakPos + 1;
                        currentLineCharsVisual = charsRestantes + 1;

                        // imprime chars restantes
                        for (int j = 0; j < charsRestantes; j++)
                            putchar(buffer[linhaStart[linhaAtual] + j]);
                        fflush(stdout);

                        bufferLen = breakPos + charsRestantes + 1; // ajusta bufferLen
                    }
                }
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    buffer[bufferLen] = '\0';
    printf(RESET);
    controlarCursor(0);
    return 0;
}


/// @brief Função para ler uma entrada numérica ASCII
/// @param buffer O buffer onde a entrada será armazenada
/// @param maxChars O número máximo de caracteres a serem lidos
/// @param color A cor do texto a ser exibido
/// @param printarExistente Se deve ou não imprimir o texto existente no buffer
/// @return O código da tecla pressionada
KeyCode inputNumeroASCII(char *buffer, int maxChars, const char *color, int printarExistente) {
    limparBufferStdin();
    controlarCursor(1); // mostra o cursor
    int len = strlen(buffer); // conta se já há algo escrito
    char c = 0;

    // imprime conteúdo existente se solicitado
    if (printarExistente && len > 0) {
        printf("%s%s" RESET, color, buffer);
        fflush(stdout);
    }

#ifdef _WIN32
    while (1) {
        // checa redimensionamento
        if (houve_resize()) return RESIZE_EVENT;

        // leitura não bloqueante
        if (_kbhit()) {
            c = getChar();

            if (c == '\r' || c == '\n') break; // Enter

            else if (c == 127 || c == '\b') {  // Backspace
                if (len > 0) {
                    len--;
                    buffer[len] = '\0';
                    printf("\b \b");
                    fflush(stdout);
                }
            }

            else if ((unsigned char)c == 27) { // ESC
                controlarCursor(0);
                printf(RESET);
                return KC_ESC;
            }

            else if (c >= '0' && c <= '9') { // apenas números
                if (len < maxChars - 1) {
                    buffer[len++] = c;
                    buffer[len] = '\0';
                    printf("%s%c" RESET, color, c);
                    fflush(stdout);
                }
            }
        }

        Sleep(1); // evita uso excessivo de CPU
    }

#else // Linux / macOS

    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    fd_set set;
    struct timeval timeout;

    while (1) {
        // checa redimensionamento
        if (houve_resize()) {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return RESIZE_EVENT;
        }

        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000; // 1ms

        int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
        if (rv > 0 && FD_ISSET(STDIN_FILENO, &set)) {
            read(STDIN_FILENO, &c, 1);

            if (c == '\r' || c == '\n') break; // Enter

            else if (c == 127 || c == '\b') {  // Backspace
                if (len > 0) {
                    len--;
                    buffer[len] = '\0';
                    printf("\b \b");
                    fflush(stdout);
                }
            }

            else if ((unsigned char)c == 27) { // ESC
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                controlarCursor(0);
                printf(RESET);
                return KC_ESC;
            }

            else if (c >= '0' && c <= '9') { // apenas números
                if (len < maxChars - 1) {
                    buffer[len++] = c;
                    buffer[len] = '\0';
                    printf("%s%c" RESET, color, c);
                    fflush(stdout);
                }
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    controlarCursor(0); // esconde o cursor
    buffer[len] = '\0';
    return 0; // deu tudo certo
}

/// @brief funcao para quebrar descricao em varias linhas
/// @param texto /// descricao original
/// @param destino /// matriz onde sera salvo o texto quebrado
/// @return  /// quantas linhas foram usadas
int quebrarDescricao(const char *texto, char destino[7][50]) {
    int linha = 0;
    const char *p = texto;

    while (*p && linha < 7) {
        int col = 0;
        int lastSpace = -1;

        while (*p && *p != '\n' && col < 49) {
            destino[linha][col] = *p;

            if (*p == ' ')
                lastSpace = col; // marca último espaço antes de encher a linha

            p++;
            col++;
        }

        
        if (*p && *p != ' ' && col >= 49 && lastSpace != -1) {
            // volta o ponteiro até o último espaço
            int retroceder = col - lastSpace - 1;
            p -= retroceder; // volta no texto original
            col = lastSpace; // corta a linha ali
        }

        // finaliza a linha
        destino[linha][col] = '\0';
        linha++;

        // pular o \n se tiver
        if (*p == '\n') p++;

        // pula espaço inicial da próxima linha, se houver
        if (*p == ' ') p++;
    }
    int rLinha = linha;
    
    // Garante que as linhas restantes fiquem vazias
    for (; linha < 7; linha++)
        destino[linha][0] = '\0';
    return rLinha;
}

// funcao para atualizar apenas uma opcao do menu
void updateOption(int linha, const char* texto, const char* fundo, const char* cor) {
    printf("\033[%d;1H\033[K", linha); // move para linha e coluna 1 e limpa a linha
    printf(" %s%s%s" RESET, fundo, cor, texto);
    fflush(stdout);
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
}

/// @brief  Função para atender um chamado
void atenderChamado(void){
   chamado* c = NULL;
    if(estruturasGlobais.chamadosAbertosComPrioridade->n > 0)
        c = (chamado*)filaPrioridadeRemover(estruturasGlobais.chamadosAbertosComPrioridade);
    else{
        c = (chamado*)filaRetirar(estruturasGlobais.chamadosAbertosSemPrioridade);
    }
    filaDuplaInserir(estruturasGlobais.chamadosAndamento,0, c);
}

/// @brief  Função para concluir um chamado
/// @param indice O índice do chamado a ser concluído
void concluirChamado(int indice){
    filaDupla* fAndamento = estruturasGlobais.chamadosAndamento;
    chamado* c = (chamado*)filaDuplaRemover(fAndamento, indice);
    filaInserir(estruturasGlobais.chamadosConcluidos, c);
}

/// @brief deletar um chamado concluido
/// @param indice O índice do chamado a ser deletado
void deletarChamado(int indice, int todos){
    if(todos){
        filaDuplaEsvaziar(estruturasGlobais.chamadosSuspensos);
    }else{
        filaDupla* fd = estruturasGlobais.chamadosAndamento;
        filaDuplaRemover(fd, indice);
    }
}

// funcao para limpar a tela
void clear(void){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}