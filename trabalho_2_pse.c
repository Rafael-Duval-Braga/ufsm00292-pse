#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* macros de testes - baseado em minUnit: www.jera.com/techinfo/jtns/jtn002.html */
#define verifica(mensagem, teste) do { if (!(teste)) return mensagem; } while (0)
#define executa_teste(teste) do { char *mensagem = teste(); testes_executados++; \
                                if (mensagem) return mensagem; } while (0)
int testes_executados = 0;

static char * executa_testes(void);

/*funcoes*/

// funcao que escreve "n" termos e armazena em um dado
char* lerDado(char dado[]) {
    char *buffer;           
    int tamanho = 1;  // tamanho inicial do buffer
    int posicao = 0;  // posição atual no buffer

    // aloca memória inicial
    buffer = (char*) malloc(sizeof(char) * tamanho);
    if (buffer == NULL) {
        return NULL;  // retorna NULL em caso de falha de alocação
    }

    // le caractere por caractere da string 'dado' até encontrar o terminador '\0'
    while (dado[posicao] != '\0') {
        // Se necessário, redimensiona o buffer
        if (posicao >= tamanho - 1) {
            tamanho *= 2;  // Dobra o tamanho do buffer
            buffer = (char*) realloc(buffer, sizeof(char) * tamanho);
            if (buffer == NULL) {
                return NULL;  // Retorna NULL em caso de falha de realocação
            }
        }
        // Armazena o caractere no buffer e incrementa a posição
        buffer[posicao] = dado[posicao];
        posicao++;
    }

    // Adiciona o terminador de string '\0'
    buffer[posicao] = '\0';

    return buffer;
}

//---------------------------------------------------------------------------

int checksum(char *dado) {  // Funcao que retorna a soma dos numeros do char
    int soma = 0;
    for (int i = 0; i < strlen(dado); i++) {
        // verifica se é um dígito numérico
        if (dado[i] >= '0' && dado[i] <= '9') {
            soma += dado[i] - '0';  // Converte caractere para valor numérico
        } else {
            return -1;  // Se houver um caractere não numérico, retorna erro
        }
    }
    return soma;
}

//---------------------------------------------------------------------------

void inttochar(int numero, char *str) {
    sprintf(str, "%d", numero);  // Funcao que converte int para char
}

//---------------------------------------------------------------------------

// Função modularizada para ser chamada no `main` ou separadamente
void codigo(char* dado_input) { 
    char *dado, qtd_dados[8], chk[8];  // Variáveis de dados e de saída
    unsigned char stx = 0x02, etx = 0x03;  // STX e ETX em formato correto
    int tamanho, intchk;

    dado = lerDado(dado_input);  // Carrega o dado de entrada
    
    printf("\n%c\n", stx);  // Exibe STX

    tamanho = strlen(dado);  // Calcula tamanho do dado
    inttochar(tamanho, qtd_dados);  // Converte o tamanho para string
    printf("%s\n", qtd_dados);  // Exibe quantidade de dados

    printf("%s\n", dado);  // Exibe o dado

    intchk = checksum(dado);  // Calcula checksum
    inttochar(intchk, chk);  // Converte o checksum para string
    printf("%s\n", chk);  // Exibe checksum
    
    printf("\n%c\n", etx);  // Exibe ETX

    free(dado);  // libera a memoria alocada
}

/*********************/

// funções de teste
static char * testeadddados(void) {
    char input[] = "123";
    char *resultado = lerDado(input);
    verifica("Erro: deveria retornar '123'", strcmp(resultado, "123") == 0);
    free(resultado);  // Libera memória alocada
    return 0;
}

static char * testeqtd(void) {
    char qtd_dados[8];
    char input[] = "123";
    int tamanho = strlen(input);
    inttochar(tamanho, qtd_dados);
    verifica("Erro: deveria retornar 3", strcmp(qtd_dados, "3") == 0);
    return 0;
}

static char * testechk(void) {
    char dado[] = "123", chk[8];
    int valor = checksum(dado);
    inttochar(valor, chk);
    verifica("Erro: deveria retornar 6", strcmp(chk, "6") == 0);
    return 0;
}

// função para rodar todos os testes
static char * executa_testes(void) {
    executa_teste(testeadddados);
    executa_teste(testeqtd);
    executa_teste(testechk);
    return 0;
}

// função principal
int main() {
    // executa testes
    char *resultado = executa_testes();
    if (resultado != 0) {
        printf("%s\n", resultado);
    } else {
        printf("TODOS OS TESTES PASSARAM\n");
    }
    printf("Testes executados: %d\n", testes_executados);

    // executa a função 'codigo' para interação manual, com entrada simulada
    char dado_teste[] = "123";  // simulação de um dado de entrada
    codigo(dado_teste);  // chama a função para processar o dado

    return resultado != 0;
}
