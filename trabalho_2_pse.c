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


//funcao que escreve "n" termos e armazena em um dado

char* lerDado(char dado[]) { //quando tem interferencia humana nao tem (char dado[]), eu adicionei char dado para poder fazer o teste
    char *buffer;           
    int tamanho = 1;  // tamanho inicial do buffer
    int posicao = 0;  // posição atual no buffer

    // aloca memória inicial
    buffer = (char*) malloc(sizeof(char) * tamanho);
    if (buffer == NULL) {
        return NULL;  // retorna NULL em caso de falha de alocação
    }

    // Lê caractere por caractere da string 'dado' até encontrar o terminador '\0'
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


int checksum(char *dado){           //funcao que retorna a soma dos numeros do char 
    int soma = 0;
    for(int i = 0; i < strlen(dado); i++){
        // verifica se é um dígito numérico
        if(dado[i] >= '0' && dado[i] <= '9'){
            soma += dado[i] - '0';  // converte caractere para valor numérico
        }
        else {
            return -1;  // se houver um caractere não numérico, retorna erro
        }
    }
    return soma;
}


//---------------------------------------------------------------------------

void inttochar(int numero, char *str) {
    
    sprintf(str, "%d", numero);     //funcao que converte int para char
}



/*
void main(void){ //codigo com interferencia humana
    
    char slot, *dado, qtd_dados[8], chk[8]; //1 byte de dados
    
    unsigned char stx[8] = "0x02", etx[8] = "0x03";
    
    int tamanho, intchk;
    
    for(;;){
        
        switch(slot){
            
            case 0: //adicionar o dado funciona
            
                printf("escreva o dado: ");
                
                dado = lerDado();   
                
                slot = 1;
                break;
     
            
            
            case 1: 
                
                printf("\n%s\n", stx);

                slot = 2;
                break;
                
                
            case 2: 
            
                tamanho = strlen(dado);
                
                inttochar(tamanho, qtd_dados);
                
                printf("%s\n", qtd_dados);
            
                slot = 3;
                break;
            
            case 3: 
            
                printf("%s\n", dado);

                slot = 4;
                break;
            
            
            case 4: 
            
                intchk = checksum(dado);
                
                inttochar(intchk, chk);
                
                printf("%s\n", chk);

                slot = 0;
                break;
                               
            default:
                slot = 0;
                break;
            
        }
        
    }
    
}
*/


/*********************/

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

static char * testeadddados(void)
{
    char input[] = "123";
    
    char *resultado = lerDado(input);
    
    verifica("Erro: deveria retornar '123'", strcmp(resultado, "123") == 0);
    
    return 0;
}

static char * testeqtd(void)
{
    
    char qtd_dados[8];
    
    char input[] = "123";
    
    int tamanho = strlen(input);
    
    inttochar(tamanho, qtd_dados);
    
    verifica("erro: deveria retornar 3", strcmp(qtd_dados, "3") == 0);
    return 0;
}


static char * testechk(void)
{
 
    char dado[] = "123", chk[8];
    
    int valor = checksum(dado);
    
    inttochar(valor, chk);
    
    verifica("erro: deveria retornar 6", strcmp(chk, "6") == 0);
    return 0;
    
}


static char * executa_testes(void)
{
    executa_teste(testeadddados);
	executa_teste(testeqtd);
	executa_teste(testechk);
	
    return 0;
}
