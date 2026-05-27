#include <stdio.h>
#include <locale.h>
#include <time.h> //Biblioteca para medir o tempo com a função clock

//declaração da constante que determina a dimensão das matrizes.
#define ORDEM 5

//protótipo das funções (para que a main fique por cima).
void exibirMatriz(int linhas, int colunas, double matriz[linhas][colunas]);
void multMatriz(double matriz1[ORDEM][ORDEM], double matriz2[ORDEM][1], double matriz3[ORDEM][1]);

void criar_matriz_solucao (double matriz_solucao[ORDEM][1]);
void criar_matriz_hilbert (double matriz[ORDEM][ORDEM]);
void criar_matriz_aumentada (double matriz1[ORDEM][ORDEM], double matriz2[ORDEM][1], double matriz3[ORDEM][ORDEM+1]);

void eliminacao (int ordem, double matriz_hilbert[ORDEM][ORDEM], double matriz_solucao[ORDEM][1]);
void substituicao_retroativa (int ordem, double matriz_hilbert[ORDEM][ORDEM], double matriz_solucao[ORDEM][1], double matriz_variavel[ORDEM][1]);
void gauss_pivot_parcial (int ordem, double matriz_aumentada[ORDEM][ORDEM+1], double matriz_variavel[ORDEM][1]);
void calcula_erro(double matriz_solucao[ORDEM][1], double solucao_encontrada[ORDEM][1], int ordem);

void main()
{
	setlocale(LC_ALL,"portuguese");
	
	double matriz_solucao[ORDEM][1];
	double matriz_hilbert[ORDEM][ORDEM];
	double matriz_aumentada[ORDEM][ORDEM+1]; //Declarando a matriz aumentada (Â = A|b)
	double matriz_variavel[ORDEM][1];
	double solucao_encontrada[ORDEM][1];
	
	//criando matriz solução (b) de ordem ORDEMx1
	criar_matriz_solucao(matriz_solucao);
	
	//Contrói a matriz hilbert de ordem ORDEMxORDEM
	criar_matriz_hilbert(matriz_hilbert);
	
	//Contrói a matriz aumentada [A|b]
	criar_matriz_aumentada(matriz_hilbert, matriz_solucao, matriz_aumentada);
		
	exibirMatriz(ORDEM, ORDEM+1, matriz_aumentada);	
	
	//Realizando a Eliminação de Gauss COM o pivotação pacial.
	//gauss_pivot_parcial(ORDEM, matriz_aumentada, matriz_variavel);
	eliminacao(ORDEM, matriz_hilbert, matriz_solucao);
	substituicao_retroativa(ORDEM, matriz_hilbert, matriz_solucao, matriz_variavel);
		
	//exibe a matriz das variáveis x[i]
	exibirMatriz(ORDEM, 1, matriz_variavel);
	
	//Testando a solução encontrada através da Eliminação de Gaus SEM o pivotação pacial.
	
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

void eliminacao (int ordem, double matriz_hilbert[ORDEM][ORDEM], double matriz_solucao[ORDEM][1])
{
	int i, j, k;
	double m;
	
	for (k=0; k<ordem; k++)
	{
		for(i=k+1; i<ordem; i++)
		{		
			m = matriz_hilbert[i][k] / matriz_hilbert[k][k];			
			
			for (j=k+1; j<ordem; j++)
			{
				matriz_hilbert[i][j] = matriz_hilbert[i][j] - (m * matriz_hilbert[k][j]);
			}
			
			matriz_solucao[i][0] = matriz_solucao[i][0] - (m * matriz_solucao[k][0]);
		}
	}
}

void substituicao_retroativa (int ordem, double matriz_hilbert[ORDEM][ORDEM], double matriz_solucao[ORDEM][1], double matriz_variavel[ORDEM][1])
{
	int i, j;
	double soma;
	
	ordem = ordem - 1; // apenas para tradução em linguagem C.
	matriz_variavel[ordem][0] = matriz_solucao[ordem][0] / matriz_hilbert[ordem][ordem];
	
	for (i=ordem-1; i>=0; i--)
	{
		soma = 0;
		
		for (j=i+1; j<=ordem; j++)
		{
			soma = (matriz_hilbert[i][j] * matriz_variavel[j][0]) + soma;
		}
		
		matriz_variavel[i][0] = (matriz_solucao[i][0] - soma) / matriz_hilbert[i][i];
	}
}

void gauss_pivot_parcial (int ordem, double matriz_aumentada[ORDEM][ORDEM+1], double matriz_variavel[ORDEM][1])
{	//Aplica o método da Eliminação Gaussiana. Esse código já inclui os processos de pivotação, de Eliminação e de Substituição Retroativa.
	////algorítmo retirado de Marcone Jamilson Freitas Souza, Departamento de Computação, Instituto de Ciências Exatas e Biológicas, Universidade Federal de Ouro Preto, 35400-000 Ouro Preto, MG, Brasil. Homepage: http://www.decom.ufop.br/prof/marcone
	int i, j, k;
	int posicao_maior_elemento;
	double valor_maior_elemento;
	double aux;
	double matriz_multiplicadores[ORDEM][ORDEM+1];
	double soma;
	
	
	for (k=0;k<ordem;k++)
	{
		valor_maior_elemento = matriz_aumentada[k][k];
		posicao_maior_elemento = k; 
		
		//Vai procurar o maior elemento da coluna k e, caso ele não seja o elemento a[k][k], realizará a permutação da linha i com a linha k. Depois, vai zerar os elementos abaixo do pivô e continuar para a próxima coluna.
		for (i=k; i<ordem; i++)
		{ 	
			if (matriz_aumentada[i][k] > valor_maior_elemento)
			{
				valor_maior_elemento = matriz_aumentada[i][k];
				posicao_maior_elemento = i; 
			}
		}
		
		for (j=k; j<=ordem; j++)
		{
			aux = matriz_aumentada[k][j];
			matriz_aumentada[k][j] = matriz_aumentada[posicao_maior_elemento][j];
			matriz_aumentada[posicao_maior_elemento][j] = aux;
		}
		
		for (i=k+1; i<ordem; i++)
		{
			matriz_multiplicadores[i][k] = matriz_aumentada[i][k] / matriz_aumentada[k][k];
			
			for (j=k+1; j<=ordem; j++)
			{
				matriz_aumentada[i][j] = matriz_aumentada[i][j] - (matriz_multiplicadores[i][k] * matriz_aumentada[k][j]);
			}
		}
	}
	
	
	ordem = ordem - 1;
	matriz_variavel[ordem][0] = matriz_aumentada[ordem][ordem+1] / matriz_aumentada[ordem][ordem];
	
	for (i=ordem-1; i>=0; i--)
	{	//Aqui está o processo de substituição retroativa
		soma = 0;
		
		for (j=i+1; j<=ordem; j++)
		{
			soma = soma + (matriz_aumentada[i][j] * matriz_variavel[j][0]);
		}
		
		matriz_variavel[i][0] = (matriz_aumentada[i][ordem+1] - soma) / matriz_aumentada[i][i];
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

