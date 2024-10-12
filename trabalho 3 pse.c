#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* macros de testes - baseado em minUnit: www.jera.com/techinfo/jtns/jtn002.html */
#define verifica(mensagem, teste) do { if (!(teste)) return mensagem; } while (0)
#define executa_teste(teste) do { char *mensagem = teste(); testes_executados++; \
                                if (mensagem) return mensagem; } while (0)
int testes_executados = 0;

#define STX 0x02
#define ETX 0x03
#define MAX_BUFFER 256

/* definição dos possíveis estados */
typedef enum {
    ST_WRITE = 0, ST_STX, ST_QTD, ST_DATA, ST_CHK, ST_ETX
} States;

/* ponteiro para funções */
typedef void (*Action)(unsigned char data);

/* estrutura da Máquina de Estados */
struct StateMachine {
    States state;
    char buffer[MAX_BUFFER];
    char chkBuffer[8]; // buffer para o checksum
    char qtdBuffer[8]; // buffer para a quantidade
    int indBuffer;
    Action action[6];  // array de ponteiros para funções
} sm;

/* função para ler dados (com a possibilidade de simular um input) */
char* lerDado(char dado[]) {
    char *buffer;
    int tamanho = 1;  // tamanho inicial do buffer
    int posicao = 0;

    // Aloca memória inicial
    buffer = (char*) malloc(sizeof(char) * tamanho);
    if (buffer == NULL) {
        return NULL;  // Retorna NULL em caso de falha de alocação
    }

    // le caractere por caractere da string 'dado'
    while (dado[posicao] != '\0') {
        // se necessario, redimensiona o buffer
        if (posicao >= tamanho - 1) {
            tamanho *= 2;  // Dobra o tamanho do buffer
            buffer = (char*) realloc(buffer, sizeof(char) * tamanho);
            if (buffer == NULL) {
                return NULL;  // retorna NULL em caso de falha de realocação
            }
        }
        buffer[posicao] = dado[posicao];
        posicao++;
    }
    buffer[posicao] = '\0';  // adiciona o terminador '\0'
    return buffer;
}

/* Função que calcula o checksum */
int checksum(char *dado) {
    int soma = 0;
    for (int i = 0; i < strlen(dado); i++) {
        if (dado[i] >= '0' && dado[i] <= '9') {
            soma += dado[i] - '0';
        } else {
            return -1;  // retorna erro se não for número
        }
    }
    return soma;
}

/* função que converte um número inteiro para string */
void inttochar(int numero, char *str) {
    sprintf(str, "%d", numero);  // converte int para string
}

/* estado ST_WRITE: Le o dado */
void stWrite(unsigned char data) {
    char dado_input[] = "123";  // simula um dado de entrada
    char* dado = lerDado(dado_input);  // le o dado
    strcpy(sm.buffer, dado);  // Copia o dado para o buffer da máquina de estados
    free(dado);  // leibera a memória alocada
    sm.state = ST_STX;  // próximo estado
}

/* Estado ST_STX: Exibe o valor de STX */
void stSTX(unsigned char data) {
    printf("\n%c\n", STX);
    sm.state = ST_QTD;  // Próximo estado
}

/* Estado ST_QTD: Calcula e exibe a quantidade de dados */
void stQtd(unsigned char data) {
    int tamanho = strlen(sm.buffer);  // Calcula a quantidade de dados
    inttochar(tamanho, sm.qtdBuffer);  // Converte para string
    printf("%s\n", sm.qtdBuffer);  // Exibe a quantidade de dados
    sm.state = ST_DATA;  // Próximo estado
}

/* Estado ST_DATA: Exibe o dado */
void stData(unsigned char data) {
    printf("%s\n", sm.buffer);  // Exibe o dado armazenado no buffer
    sm.state = ST_CHK;  // Próximo estado
}

/* Estado ST_CHK: Calcula e exibe o checksum */
void stChk(unsigned char data) {
    int valor = checksum(sm.buffer);  // Calcula o checksum
    inttochar(valor, sm.chkBuffer);  // Converte o checksum para string
    printf("%s\n", sm.chkBuffer);  // Exibe o checksum
    sm.state = ST_ETX;  // Próximo estado
}

/* Estado ST_ETX: Exibe o valor de ETX */
void stETX(unsigned char data) {
    printf("\n%c\n", ETX);
    sm.state = ST_WRITE;  // Reinicia o processo
}

/* Função para processar os estados */
void handleRx(unsigned char data) {
    sm.action[sm.state](data);  // Chama a função correspondente ao estado atual
}

/* Inicializa a máquina de estados */
void initSM() {
    sm.state = ST_WRITE;
    sm.indBuffer = 0;

    // Define as funções para cada estado
    sm.action[ST_WRITE] = stWrite;
    sm.action[ST_STX] = stSTX;
    sm.action[ST_QTD] = stQtd;
    sm.action[ST_DATA] = stData;
    sm.action[ST_CHK] = stChk;
    sm.action[ST_ETX] = stETX;
}

/****************** Funções de Teste ******************/

// Testa a função de leitura de dado
static char *testeLerDado(void) {
    char input[] = "123";
    char *resultado = lerDado(input);
    verifica("Erro: deveria retornar '123'", strcmp(resultado, "123") == 0);
    free(resultado);  // Libera a memória alocada
    return 0;
}

// Testa a função de conversão de quantidade
static char *testeQtd(void) {
    char qtd_dados[8];
    char input[] = "123";
    int tamanho = strlen(input);
    inttochar(tamanho, qtd_dados);
    verifica("Erro: deveria retornar '3'", strcmp(qtd_dados, "3") == 0);
    return 0;
}

// Testa a função de checksum
static char *testeChk(void) {
    char dado[] = "123", chk[8];
    int valor = checksum(dado);
    inttochar(valor, chk);
    verifica("Erro: deveria retornar 6", strcmp(chk, "6") == 0);
    return 0;
}

// Função para executar todos os testes
static char *executa_testes(void) {
    executa_teste(testeLerDado);
    executa_teste(testeQtd);
    executa_teste(testeChk);
    return 0;
}

/****************** Função Principal ******************/

int main() {
    // Executa os testes
    char *resultado = executa_testes();
    if (resultado != 0) {
        printf("%s\n", resultado);
    } else {
        printf("TODOS OS TESTES PASSARAM\n");
    }
    printf("Testes executados: %d\n", testes_executados);

    // Inicializa e executa a máquina de estados
    initSM();  // Inicializa a máquina de estados
    for (int i = 0; i < 6; i++) {
        handleRx(0);  // Processa a máquina de estados
    }

    return resultado != 0;
}
