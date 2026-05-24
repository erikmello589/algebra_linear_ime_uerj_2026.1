// Algebra Linear: Noite. Alunos: Erik Mello e Gabriel da Cruz.
#include <stdio.h>
#include <locale.h>

//declaração de constante
#define ORDEM 15
#define FALSE -1
#define TRUE 1

/*1 - Soma de Vetores (Vetor1 + Vetor2 = vetor3);
2 - Multiplicação Vetor x Escalar (Vetor1 * E = Vetor3);
3 - Multiplicação Vetor x Vetor (Vetor1 * Vetor2 = Int);
4 - Soma Matriz + Matriz (Matriz1 + Matriz2 = Matriz3);
5 - Multiplicação Matriz x Escalar (Matriz * E = Matriz2);
6 - Multiplicação Matriz x Matriz (Matriz1 * Matriz2 = Matriz3) */

//protótipo das funções (para que a main fique por cima)
void exibirVetor (int vetor[], int tamanhoVetor);
void criar_matriz_solucao (int matriz_solucao[ORDEM][1]);
void criar_matriz_hilbert (int matriz[ORDEM][ORDEM]);


void exibirMatriz (int matriz[ORDEM][1]);

void somaVetores(int vetor1[], int vetor2[], int ORDEMVetores); // vai retornar um vetor3 
void multVEscalar(int vetor1[], float escalar, int ORDEMVetor); // vai retornar um vetor3
void multVetor(int vetor1[], int vetor2[], int ORDEMVetores); // vai retornar um int

void somaMatrizes(int matriz1[ORDEM][ORDEM], int matriz2[ORDEM][ORDEM], int ORDEMMatrizes); // vai retornar uma matriz3
void multMEscalar(int matriz1[ORDEM][ORDEM], float escalar, int ORDEMMatriz); // vai retornar uma matriz3
void multMatriz(int matriz1[ORDEM][ORDEM], float escalar, int ORDEMMatriz); // vai retornar uma matriz3

void eliminacao (int ordem, int matriz_hilbert[ORDEM][ORDEM], int matriz_solucao[ORDEM][1]);
void substituicao_retroativa (int ordem, int matriz_hilbert[ORDEM][ORDEM], int matriz_solucao[ORDEM][1], int matriz_variavel[ORDEM][1]);

void main()
{
	setlocale(LC_ALL,"portuguese");
	
	int i, j, soma;
	int matriz_hilbert[ORDEM][ORDEM];
	int matriz_solucao[ORDEM][1];
	int matriz_variavel[ORDEM][1];
	
	//criando matriz hilbert
	for (i=0;i<ORDEM;i++)
	{
		for (j=0;j<ORDEM;j++)
		{
			soma = (i + 1) + (j + 1) - 1;
			matriz_hilbert[i][j] = 1 / soma;
		}		
	}
	
	//criando matriz solucao
	criar_matriz_solucao(matriz_solucao);
	
	eliminacao(ORDEM, matriz_hilbert, matriz_solucao);
	
	substituicao_retroativa(ORDEM, matriz_hilbert, matriz_solucao, matriz_variavel);
	
	exibirMatriz(matriz_variavel);
}

void somaVetores(int vetor1[], int vetor2[], int ORDEMVetores) // vai exibir um vetor resultante
{
	int vetor3[ORDEMVetores];
	int i;
	
	for	(i = 0; i < ORDEMVetores; i++)
	{
		vetor3[i] = vetor1[i] + vetor2[i]; 
	}
	exibirVetor(vetor3, ORDEMVetores);
}

void multVEscalar(int vetor1[], float escalar, int ORDEMVetor) // vai retornar um vetor resultante
{
	int vetorResultante[ORDEMVetor];
	int i;
	
	for (i = 0; i < ORDEMVetor; i++)
	{
		vetorResultante[i] = escalar * vetor1[i];
	}
	
	exibirVetor(vetorResultante, ORDEMVetor);
	
}

void multVetor(int vetor1[], int vetor2[], int ORDEMVetores) // vai retornar um int
{
	int produto_interno = 0;
	int i;
	int multiplicacao;
	
	for (i = 0; i < ORDEMVetores; i++)
	{
		multiplicacao = vetor1[i] * vetor2[i];
		produto_interno = produto_interno + multiplicacao;
	}
	
	printf("Valor do Produto interno: %i ", produto_interno);
}

void exibirVetor (int vetor[], int ORDEMVetor)
{
	int i;
	
	printf ("\nElementos do vetor: ");
	
	for (i=0;i<ORDEMVetor;i++)
	{
		printf ("%d ", vetor[i]);
	}
	
	printf ("\n\n");
}

void criar_matriz_solucao (int matriz_solucao[ORDEM][1])
{
	int i, j;
			
	for (i=0;i<ORDEM;i++)
	{
		for (j=0;j<1;j++)
		{
			matriz_solucao[i][j] = 1;
		}
	}
}

void exibirMatriz (int matriz[ORDEM][1])
{
	int i, j;
	
	printf ("\nElementos da matriz:\n\n");
	
	//varrendo as linhas da matriz
	for (i=0;i<ORDEM;i++)
	{
		//varrendo as colunas
		for (j=0;j<1;j++)
		{
			printf ("%i ", matriz[i][j]);
		}
		
		printf ("\n");
	}
	
	printf ("\n\n");
}

void eliminacao (int ordem, int matriz_hilbert[ORDEM][ORDEM], int matriz_solucao[ORDEM][1])
{
	int i, j, k, m;
	int negativo;
	
	for (k=0; k<ordem; k++)
	{
		for(i=k+1; i<ordem; i++)
		{
			//negativo = matriz_hilbert[i][k] * -1;
			//m = negativo / matriz_hilbert[k][k];
			
			if (matriz_hilbert[k][k] == 0)
			{
				m = 0;
			}
			else
			{
				m = matriz_hilbert[i][k] / matriz_hilbert[k][k];
			}			
			
			for (j=k+1; j<ordem; j++)
			{
				matriz_hilbert[i][j] = matriz_hilbert[i][j] - (m * matriz_hilbert[k][j]);
			}
			
			matriz_solucao[i][0] = matriz_solucao[i][0] - (m * matriz_solucao[k][0]);
		}
	}
}

void substituicao_retroativa (int ordem, int matriz_hilbert[ORDEM][ORDEM], int matriz_solucao[ORDEM][1], int matriz_variavel[ORDEM][1])
{
	int i, j;
	int soma;
	ordem = ordem - 1;
	
	if(matriz_hilbert[ordem][ordem] == 0)
	{
		matriz_variavel[ordem][0] = 0;
	}
	else
	{
		matriz_variavel[ordem][0] = matriz_solucao[ordem][0] / matriz_hilbert[ordem][ordem];
	}
	
	
	for (i=ordem-1; i>=0; i--)
	{
		soma = 0;
		
		for (j=i+1; j<=ordem; j++)
		{
			soma = (matriz_hilbert[i][j] * matriz_variavel[j][0]) + soma;
		}
		
		if(matriz_hilbert[i][i] == 0)
		{
			matriz_variavel[i][0] = 0;
		}
		else
		{
			matriz_variavel[i][0] = (matriz_solucao[i][0] - soma) / matriz_hilbert[i][i];
		}
	}
}
