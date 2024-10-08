#include <stdio.h>
#include <stdlib.h>

//feito por Rafael Duval Braga

/* macros de testes - baseado em minUnit: www.jera.com/techinfo/jtns/jtn002.html */
#define verifica(mensagem, teste) do { if (!(teste)) return mensagem; } while (0)
#define executa_teste(teste) do { char *mensagem = teste(); testes_executados++; \
                                if (mensagem) return mensagem; } while (0)
int testes_executados = 0;

static char * executa_testes(void);

typedef struct {
	int *buffer;  // array que ira armazenar os valores do buffer
	int size;     // tamanho do buffer
	int begin;    // inC-cio (primeiro elemento)
	int end;      // fim
	int full;     // flag para indicar se o buffer esta cheio
} Buffer;

// funcao para criar o buffer
Buffer *criar(int csize) {
	Buffer *buffer = malloc(sizeof(Buffer));  // aloca o espaco para o struct buffer
	buffer->buffer = malloc(sizeof(int) * csize);  // aloca o espaC'o para o array de inteiros
	buffer->size = csize;
	buffer->begin = 0;
	buffer->end = 0;
	buffer->full = 0;  // inicialmente, o buffer n estC! cheio
	return buffer;
}

// funcao para criar o buffer e retornar o size no teste
Buffer *criar_retornasize(int csize) {
	Buffer *buffer = malloc(sizeof(Buffer));  // aloca o espaco para o struct buffer
	buffer->buffer = malloc(sizeof(int) * csize);  // aloca o espaC'o para o array de inteiros
	buffer->size = csize;
	buffer->begin = 0;
	buffer->end = 0;
	buffer->full = 0;  // inicialmente, o buffer n estC! cheio
	return buffer->size;
}


// funC'C#o para adicionar valores no buffer
int adiciona(int x, Buffer *buffer) {
	if (buffer->full) {  // verifica se o buffer estC! cheio
		printf("erro: o buffer estC! cheio!\n");
		return -1; // fracasso
	}

	buffer->buffer[buffer->end] = x;  // adiciona o valor no fim do buffer
	buffer->end = (buffer->end + 1) % buffer->size;  // incrementa o fim, circulando se necessario

	if (buffer->end == buffer->begin) {  // se a posiC'C#o de fim atingir a posiC'C#o de inC-cio, o buffer estC! cheio
		buffer->full = 1;
	}

	return 1;  // sucesso
}

// funcao para ler e remover valores do buffer
int leitura(Buffer *buffer) {
	if (buffer->begin == buffer->end && !buffer->full) {  // Verifica se o buffer estC! vazio
//		printf("Erro: o buffer estC! vazio!\n");
		return -1;
	}

	int valor = buffer->buffer[buffer->begin];  // le o valor na posiC'C#o de inC-cio
//	printf("Valor lido: %d\n", valor);

	buffer->begin = (buffer->begin + 1) % buffer->size;  // Incrementa o inC-cio, circulando se necessario

	buffer->full = 0;  // Depois de ler, o buffer nao estC! mais cheio

	return valor;
}

// funC'C#o para exibir o conteC:do do buffer
void exibir(Buffer *buffer) {
	if (buffer->begin == buffer->end && !buffer->full) {
		printf("O buffer estC! vazio.\n");
		return;
	}

	printf("ConteC:do do buffer: ");
	int i = buffer->begin;
	while (i != buffer->end || (buffer->full && i == buffer->end)) {
		printf("%d ", buffer->buffer[i]);
		i = (i + 1) % buffer->size;
		if (i == buffer->end && buffer->full) break;
	}
	printf("\n");
}

void liberar_memoria_alocada(){
    
    
    
}


/*
codigo main para interferencia humana

int main() {
    int csize, x, choose;

    //printf("Escreva o tamanho do buffer: ");
    //scanf("%d", &csize); 

    Buffer *buffer = criar(csize);  // cria o buffer com o tamanho especificado

    while (1) {  // Loop para operações de adicionar/ler/exibir
        printf("Digite 1 para adicionar ao buffer, 2 para ler e remover, 3 para exibir o buffer: ");
        scanf("%d", &choose);

        if (choose == 1) {
            //printf("Escreva o valor que deseja adicionar: ");
            scanf("%d", &x);

            adiciona(x, buffer);  // adiciona o valor ao buffer
        }
        else if (choose == 2) {
            leitura(buffer);  // le e remove o valor do buffer
        }
        else if (choose == 3) {
            exibir(buffer);  // exibe o conteúdo do buffer
        }
        else {
            printf("Valor inválido!\n");
        }
       
        int final = buffer->end;
       
    }

    // liberar a memória alocada
    free(buffer->buffer);
    free(buffer);

    return 0;
}*/


/***********************************************/

int main()
{
	char *resultado = executa_testes();
	if (resultado != 0)
	{
		printf("%s\n", resultado);
	}
	else
	{
		printf("TODOS OS TESTES PASSARAM\n");
	}
	printf("Testes executados: %d\n", testes_executados);

	getch();
	return resultado != 0;
}

/* Teste de exemplo - apenas para demonstraC'C#o 


static int retornax (int x)
{
	return x;
}

static char * teste_retorna1_caso_recebe1(void)
{
	verifica("erro: deveria retornar 1", retornax(1) == 1);
	return 0;
}

*/

static char * teste_tamanho(void)         //tamanho retorna 4
{
    
    Buffer * buffer = criar_retornasize(4);
    
	verifica("erro: deveria retornar 4", buffer == 4); //criar(int csize)
	return 0;
}


static char * teste_insercao(void)  //retorna escrita do buffer
{
    Buffer * buffer = criar(4);
    int sucesso = adiciona(2, buffer); //int adiciona(int x, Buffer *buffer)
    
	verifica("erro: deveria retornar 1", sucesso == 1); //retorna 0 quer dizer sucesso retorna -1 = fracasso
	return 0;
}


static char * teste_criacao(void)         //criar buffer
{
    
    Buffer * buffer = criar(4);
    
	verifica("erro: buffer nao criado", buffer != NULL); //se buffer nao foi nulo, ele nao foi criado
	return 0;
}



static char * teste_leitura(void)         //leitura de buffer
{
    Buffer *buffer = criar(4); 
    adiciona(10, buffer);       
    adiciona(20, buffer);       
    
    int valor1 = leitura(buffer);  // Ler o primeiro valor do buffer
    verifica("erro: deveria retornar 10", valor1 == 10);  // Verificar se o valor lido é 10
    
    int valor2 = leitura(buffer);  // Ler o segundo valor do buffer
    verifica("erro: deveria retornar 20", valor2 == 20);  // Verificar se o valor lido é 20
    
    return 0;
}

/***********************************************/


static char * executa_testes(void)
{
	executa_teste(teste_tamanho);
	executa_teste(teste_insercao);
	executa_teste(teste_criacao);
	executa_teste(teste_leitura); 
	
	//executa_teste(teste_retorna1_caso_recebe1);
	
	return 0;
}