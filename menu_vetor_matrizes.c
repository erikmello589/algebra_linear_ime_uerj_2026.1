// Algebra Linear: Noite. Alunos: Erik Mello e Gabriel da Cruz.
#include <stdio.h>
#include <locale.h>

//declaração de constante
#define ORDEM 3
#define FALSE -1
#define TRUE 1

//protótipo das funções (para que a main fique por cima)
void exibirVetor (int vetor[], int tamanhoVetor);
void exibirMatriz(int linhas, int colunas, float matriz[linhas][colunas]);

void somaVetores(int vetor1[], int vetor2[], int ORDEMVetores);
void multVEscalar(int vetor1[], float escalar, int ORDEMVetor);
void multVetor(int vetor1[], int vetor2[], int ORDEMVetores); 
void somaMatrizes(int matriz1[ORDEM][ORDEM], int matriz2[ORDEM][ORDEM], int ORDEMMatrizes); 
void multMEscalar(int matriz1[ORDEM][ORDEM], float escalar, int ORDEMMatriz); 

void multMatriz(float matriz1[ORDEM][ORDEM], float matriz2[ORDEM][1], float matriz3[ORDEM][1]);

void criar_matriz_solucao (float matriz_solucao[ORDEM][1]);
void criar_matriz_hilbert (float matriz[ORDEM][ORDEM]);
void criar_matriz_aumentada (float matriz1[ORDEM][ORDEM], float matriz2[ORDEM][1], float matriz3[ORDEM][ORDEM+1]);

void eliminacao (int ordem, float matriz_hilbert[ORDEM][ORDEM], float matriz_solucao[ORDEM][1]);
void substituicao_retroativa (int ordem, float matriz_hilbert[ORDEM][ORDEM], float matriz_solucao[ORDEM][1], float matriz_variavel[ORDEM][1]);
void gauss_pivoteamento_parcial (int ordem, float matriz_aumentada[ORDEM][ORDEM+1], float matriz_variavel[ORDEM][1]);

void main()
{
	setlocale(LC_ALL,"portuguese");
	
	float matriz_solucao[ORDEM][1];
	float matriz_hilbert[ORDEM][ORDEM];
	float matriz_aumentada[ORDEM][ORDEM+1]; //Declarando a matriz aumentada (Â = A|b)
	float matriz_variavel[ORDEM][1];
	float solucao_encontrada[ORDEM][1];
	
	//criando matriz solucao
	criar_matriz_solucao(matriz_solucao);
	
	//criando matriz hilbert
	criar_matriz_hilbert(matriz_hilbert);
	
	//criando matriz aumentada
	criar_matriz_aumentada(matriz_hilbert, matriz_solucao, matriz_aumentada);
		
	/*
	//Realizando a Eliminação de Gauss SEM o pivoteamento pacial.
	eliminacao(ORDEM, matriz_hilbert, matriz_solucao);
	substituicao_retroativa(ORDEM, matriz_hilbert, matriz_solucao, matriz_variavel);
	exibirMatriz(matriz_variavel);
	
	//Testando a solução encontrada através da Eliminação de Gaus SEM o pivoteamento pacial.
	criar_matriz_hilbert(matriz_hilbert);
	multMatriz(matriz_hilbert, matriz_variavel, solucao_encontrada);
	exibirMatriz(solucao_encontrada);
	*/

	//Realizando a Eliminação de Gauss COM o pivoteamento pacial.
	gauss_pivoteamento_parcial(ORDEM, matriz_aumentada, matriz_variavel);
	exibirMatriz(ORDEM, 1, matriz_variavel);
	
	//Testando a solução encontrada através da Eliminação de Gaus COM o pivoteamento pacial.
	criar_matriz_hilbert(matriz_hilbert);
	multMatriz(matriz_hilbert, matriz_variavel, solucao_encontrada);
	exibirMatriz(ORDEM, 1, solucao_encontrada);

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

void multMatriz(float matriz1[ORDEM][ORDEM], float matriz2[ORDEM][1], float matriz3[ORDEM][1])
{
	int i, j, k; 
	
	// 1. Verificação da regra matemática estrita
    /*if (colunas1 != linhas2) 
	{
        printf("Erro Matemático: Impossível multiplicar. Colunas de A (%d) != Linhas de B (%d).\n", colunasA, linhasB);
        return false; // Aborta a função e retorna falha
    }*/

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

void criar_matriz_hilbert (float matriz[ORDEM][ORDEM])
{
	int i, j;
	float soma;
	
	for (i=0;i<ORDEM;i++)
	{
		for (j=0;j<ORDEM;j++)
		{
			soma = (i + 1) + (j + 1) - 1;
			matriz[i][j] = 1 / soma;
		}		
	}
}

void criar_matriz_solucao (float matriz_solucao[ORDEM][1])
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

void criar_matriz_aumentada (float matriz1[ORDEM][ORDEM], float matriz2[ORDEM][1], float matriz3[ORDEM][ORDEM+1])
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

void exibirMatriz(int linhas, int colunas, float matriz[linhas][colunas])
{
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

void eliminacao (int ordem, float matriz_hilbert[ORDEM][ORDEM], float matriz_solucao[ORDEM][1])
{
	int i, j, k;
	float m;
	
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

void substituicao_retroativa (int ordem, float matriz_hilbert[ORDEM][ORDEM], float matriz_solucao[ORDEM][1], float matriz_variavel[ORDEM][1])
{
	int i, j;
	float soma;
	
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

void gauss_pivoteamento_parcial (int ordem, float matriz_aumentada[ORDEM][ORDEM+1], float matriz_variavel[ORDEM][1])
{
	int i, j, k;
	int posicao_maior_elemento;
	float valor_maior_elemento;
	float aux;
	float matriz_multiplicadores[ORDEM][ORDEM+1];
	float soma;
	
	for (k=0;k<ordem;k++)
	{
		valor_maior_elemento = matriz_aumentada[k][k];
		posicao_maior_elemento = k; 
		
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
	{
		soma = 0;
		
		for (j=i+1; j<=ordem; j++)
		{
			soma = soma + (matriz_aumentada[i][j] * matriz_variavel[j][0]);
		}
		
		matriz_variavel[i][0] = (matriz_aumentada[i][ordem+1] - soma) / matriz_aumentada[i][i];
	}	
}
