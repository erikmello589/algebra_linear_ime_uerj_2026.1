#include <stdio.h>
#include <locale.h>

//declaração da constante que determina a dimensão das matrizes.
#define ORDEM 5

//protótipo das funções (para que a main fique por cima).
void exibirMatriz(int linhas, int colunas, double matriz[linhas][colunas]);
void multMatriz(double matriz1[ORDEM][ORDEM], double matriz2[ORDEM][1], double matriz3[ORDEM][1]);
void criar_matriz_solucao (double matriz_solucao[ORDEM][1]);
void criar_matriz_hilbert (double matriz[ORDEM][ORDEM]);
void criar_matriz_aumentada (double matriz1[ORDEM][ORDEM], double matriz2[ORDEM][1], double matriz3[ORDEM][ORDEM+1]);
void calcula_erro(double matriz_solucao[ORDEM][1], double solucao_encontrada[ORDEM][1], int ordem);

void gauss_jordan (int ordem, double matriz_hilbert[ORDEM][ORDEM], double matriz_solucao[ORDEM][1],  double matriz_aumentada[ORDEM][ORDEM+1]);

//Funções para implementar a Eliminação de Gauss SEM o pivoteamento parcial
void eliminacao (int ordem, double matriz_hilbert[ORDEM][ORDEM], double matriz_solucao[ORDEM][1]);
void substituicao_retroativa (int ordem, double matriz_hilbert[ORDEM][ORDEM], double matriz_solucao[ORDEM][1], double matriz_variavel[ORDEM][1]);



void main()
{
	setlocale(LC_ALL,"portuguese");
	
	int i;
	
	double matriz_solucao[ORDEM][1];
	double matriz_hilbert[ORDEM][ORDEM];
	double matriz_aumentada[ORDEM][ORDEM+1]; //Declarando a matriz aumentada (Â = A|b)
	double matriz_variavel[ORDEM][1];
	double solucao_encontrada[ORDEM][1];
	
	//criando matriz solução (b) de ordem ORDEMx1
	criar_matriz_solucao(matriz_solucao);
	
	//Contrói a matriz hilbert de ordem ORDEMxORDEM
	criar_matriz_hilbert(matriz_hilbert);
	
	//Realizando o metodo Gauss-Jordan.
	gauss_jordan(ORDEM, matriz_hilbert, matriz_solucao, matriz_aumentada);
		
	//Implentar forma de matriz_variavel = ultima coluna de matriz_aumentada
	for (i=0; i<ORDEM; i++)
	{
		matriz_variavel[i][0] = matriz_aumentada[i][ORDEM];
	}
		
	//exibe a matriz das variáveis x[i]
	exibirMatriz(ORDEM, 1, matriz_variavel);
		
	//refaz a matriz de Hilbert na forma original
	criar_matriz_hilbert(matriz_hilbert);
	
	//refaz a matriz solucao na forma original
	criar_matriz_solucao(matriz_solucao);
	
	//Realiza a multiplicação A*x para encontrar o b aproximado da nossa solução
	multMatriz(matriz_hilbert, matriz_variavel, solucao_encontrada);
	
	//Exibe o b encontrado
	exibirMatriz(ORDEM, 1, solucao_encontrada);
	
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

void criar_matriz_aumentada (double matriz1[ORDEM][ORDEM], double matriz2[ORDEM][1], double matriz3[ORDEM][ORDEM+1])
{
	int i, j;
	
	//percorrendo hilbert
	for (i=0; i<ORDEM; i++)
	{
		for(j=0; j<ORDEM; j++)
		{
			matriz3[i][j] = matriz1[i][j];
		}
	}
	
	//percorrido toda hilbert, então, agora colocar a solucao
	for (i=0; i<ORDEM; i++)
	{
		matriz3[i][j] = matriz2[i][0];
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

void eliminacao (int ordem, double matriz_hilbert[ORDEM][ORDEM], double matriz_solucao[ORDEM][1])
{
	int i, j, k;
	double m;
	
	for (k=0; k<ordem; k++)
	{
		for(i=k+1; i<ordem; i++)
		{		
			m = matriz_hilbert[i][k] / matriz_hilbert[k][k];			
			
			for (j=k; j<ordem; j++)
			{
				matriz_hilbert[i][j] = matriz_hilbert[i][j] - (m * matriz_hilbert[k][j]);
			}
			
			matriz_solucao[i][0] = matriz_solucao[i][0] - (m * matriz_solucao[k][0]);
		}
	}
}

void gauss_jordan (int ordem, double matriz_hilbert[ORDEM][ORDEM], double matriz_solucao[ORDEM][1],  double matriz_aumentada[ORDEM][ORDEM+1])
{
	int i, j, k;
	double pivo;
	double multiplicacao;
	
	eliminacao(ordem, matriz_hilbert, matriz_solucao);	
	criar_matriz_aumentada(matriz_hilbert, matriz_solucao, matriz_aumentada);
	
	for (i=ordem-1; i>=0; i--)
	{
		for (j=i-1; j>=0; j--)
		{
			pivo = matriz_aumentada[j][i] / matriz_aumentada[i][i];

			for (k=ordem; k>=i; k--)
			{
				multiplicacao = pivo * matriz_aumentada[i][k];
				matriz_aumentada[j][k] = matriz_aumentada[j][k] - multiplicacao;
			}
		}
		
		for (j=ordem; j>=i; j--)
		{
			matriz_aumentada[i][j] = matriz_aumentada[i][j] / matriz_aumentada[i][i];
		}
	}
}
