#include <stdio.h>
#include <locale.h>

//declaração da constante que determina a dimensão das matrizes.
#define ORDEM 5

//protótipo das funções (para que a main fique por cima).
void exibirMatriz(int linhas, int colunas, double matriz[linhas][colunas]);
void multMatriz(double matriz1[ORDEM][ORDEM], double matriz2[ORDEM][1], double matriz3[ORDEM][1]);
void criar_matriz_solucao (double matriz_solucao[ORDEM][1]);
void criar_matriz_hilbert (double matriz[ORDEM][ORDEM]);
void calcula_erro(double matriz_solucao[ORDEM][1], double solucao_encontrada[ORDEM][1], int ordem);

//Funções para implementar o método LU de decomposição
void substituicao_retroativa (int ordem, double matriz_hilbert[ORDEM][ORDEM], double matriz_solucao[ORDEM][1], double matriz_variavel[ORDEM][1]);
void substituicao_progressiva(int ordem, double matriz_L[ORDEM][ORDEM], double matriz_solucao[ORDEM][1], double matriz_Y[ORDEM][1]);
void decomposicao_LU (int ordem, double matriz_hilbert[ORDEM][ORDEM], double matriz_L[ORDEM][ORDEM], double matriz_U[ORDEM][ORDEM]);

void main()
{
	setlocale(LC_ALL,"portuguese");
	
	double matriz_solucao[ORDEM][1];
	double matriz_hilbert[ORDEM][ORDEM];
	double matriz_aumentada[ORDEM][ORDEM+1]; //Declarando a matriz aumentada (Â = A|b)
	double matriz_variavel[ORDEM][1];
	double solucao_encontrada[ORDEM][1];
	
	double matriz_Y[ORDEM][1];
	
	double matriz_L[ORDEM][ORDEM];
	double matriz_U[ORDEM][ORDEM];
	
	//criando matriz solução (b) de ordem ORDEMx1
	criar_matriz_solucao(matriz_solucao);
	
	//Contrói a matriz hilbert de ordem ORDEMxORDEM
	criar_matriz_hilbert(matriz_hilbert);
	
	//Realizando o método LU de Decomposição.
	decomposicao_LU(ORDEM, matriz_hilbert, matriz_L, matriz_U);
		
	//exibe as matrizes L e U
	exibirMatriz(ORDEM, ORDEM, matriz_L);
	exibirMatriz(ORDEM, ORDEM, matriz_U);
	
	//refaz a matriz de Hilbert na forma original
	criar_matriz_hilbert(matriz_hilbert);
	
	//refaz a matriz solucao na forma original
	criar_matriz_solucao(matriz_solucao);
	
	substituicao_progressiva(ORDEM, matriz_L, matriz_solucao, matriz_Y);
	substituicao_retroativa(ORDEM, matriz_U, matriz_Y, matriz_variavel);
	
	//Realiza a multiplicação A*x para encontrar o b aproximado da nossa solução
	multMatriz(matriz_hilbert, matriz_variavel, solucao_encontrada);
	
	//Exibe o b encontrado
	exibirMatriz(ORDEM, 1, matriz_variavel);
	
	calcula_erro(matriz_solucao, solucao_encontrada, ORDEM);
}

void multMatriz(double matriz1[ORDEM][ORDEM], double matriz2[ORDEM][1], double matriz3[ORDEM][1])
{	//Multiplicação de matrizes padrão
	int i, j, k; 
	
    for (i = 0; i < ORDEM; i++) 
	{
        for (j = 0; j < 1; j++) 
		{
            matriz3[i][j] = 0;
            
            for ( k = 0; k < ORDEM; k++) 
			{
                matriz3[i][j] = matriz3[i][j] + matriz1[i][k] * matriz2[k][j];
            }
        }
    }
}

void criar_matriz_hilbert (double matriz[ORDEM][ORDEM])
{	//Preenche a matriz_hilbert[ORDEM][ORDEM]
	int i, j;
	double soma;
	
	for (i=0;i<ORDEM;i++)
	{
		for (j=0;j<ORDEM;j++)
		{
			soma = (i + 1) + (j + 1) - 1;
			matriz[i][j] = 1 / soma;
		}		
	}
}

void criar_matriz_solucao (double matriz_solucao[ORDEM][1])
{	//Preenche a matriz_solucao[ORDEM][1] (b), do sistema Ax=b, com todas as entradas iguais a 1.
	int i, j;
			
	for (i=0;i<ORDEM;i++)
	{
		for (j=0;j<1;j++)
		{
			matriz_solucao[i][j] = 1;
		}
	}
}

void exibirMatriz(int linhas, int colunas, double matriz[linhas][colunas])
{	// funcão exibir genérica para facilitar a visualização dos resultados.
	int i, j;
	
	printf("\nElementos da matriz:\n\n");
	
	// varrendo as linhas da matriz
	for (i = 0; i < linhas; i++)
	{
		// varrendo as colunas da matriz
		for (j = 0; j < colunas; j++)
		{
			printf("%.10f ", matriz[i][j]);
		}
		
		printf("\n");
	}
	
	printf("\n\n");
}

void decomposicao_LU(int ordem, double matriz_hilbert[ORDEM][ORDEM], double matriz_L[ORDEM][ORDEM], double matriz_U[ORDEM][ORDEM])
{
	int i, j, k;
	double fracao;
	double multiplicacao;
	
	// U <- A (matriz_U iniciando igual a hilbert) (botar como limitação da linguagem C).
	for (i = 0; i < ordem; i++) 
	{
        for (j = 0; j < ordem; j++) 
		{
            matriz_U[i][j] = matriz_hilbert[i][j];
        }
    }
    
    for (i = 0; i < ordem; i++) 
	{
        for (j = i+1; j < ordem; j++) 
		{
			fracao = matriz_U[j][i] / matriz_U[i][i];
            matriz_L[j][i] = fracao;
            
            for (k = ordem; k >= i; k--) 
            {
            	multiplicacao = matriz_U[i][k] * matriz_L[j][i];
            	matriz_U[j][k] = matriz_U[j][k] - multiplicacao;
			}
        }
    }
    
    //Preenchendo diagonal principal de L com '1'. (botar como observação).
    for (i=0; i < ordem; i++)
    {
    	matriz_L[i][i] = 1;
	}
}

void substituicao_progressiva(int ordem, double matriz_L[ORDEM][ORDEM], double matriz_solucao[ORDEM][1], double matriz_Y[ORDEM][1])
{
	int i, j;
	double soma;
	double multiplicacao;
	double subtracao;
	
	for (i=0; i<ordem; i++)
	{
		soma = 0;
		
		for (j=0; j<=i-1; j++)
		{
			multiplicacao = matriz_L[i][j] * matriz_Y[j][0];
			soma = soma + multiplicacao;
		}
		
		subtracao = matriz_solucao[i][0] - soma;
		matriz_Y[i][0] = subtracao / matriz_L[i][i];
	}
}

void substituicao_retroativa (int ordem, double matriz_U[ORDEM][ORDEM], double matriz_Y[ORDEM][1], double matriz_variavel[ORDEM][1])
{
	int i, j;
	double soma;
	
	ordem = ordem - 1; // apenas para tradução em linguagem C.
	matriz_variavel[ordem][0] = matriz_Y[ordem][0] / matriz_U[ordem][ordem];
	
	for (i=ordem-1; i>=0; i--)
	{
		soma = 0;
		
		for (j=i+1; j<=ordem; j++)
		{
			soma = (matriz_U[i][j] * matriz_variavel[j][0]) + soma;
		}
		
		matriz_variavel[i][0] = (matriz_Y[i][0] - soma) / matriz_U[i][i];
	}
}

void calcula_erro(double matriz_solucao[ORDEM][1], double solucao_encontrada[ORDEM][1], int ordem)
{
	double sqm;
	double somatorio = 0;
	double subtracao;
	double quadrado;
	double fracao = 1.0 / ordem;
	int i, j;
	
	for (i = 0; i < ordem; i++)
	{
		for (j = 0; j < 1; j++)
		{
				subtracao = matriz_solucao[i][j] - solucao_encontrada[i][j];
				quadrado = subtracao * subtracao;
				somatorio = somatorio + quadrado;
		}
	}
	
	sqm = fracao * somatorio;
	
	printf("Erro calculado: %e", sqm);
    
}

