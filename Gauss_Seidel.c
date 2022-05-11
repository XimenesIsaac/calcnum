/*Matematica numerica
Claudir Oliveira
Método de Gauss Seidel
*/
/*
<1º passo-Criação de ponteiros e alocação de ponteiros>
<2º passo-Criar ponteiro para arquivos e escrever aquivos>
############################################################
compilar:gcc SeidelModificado.c -o <name>
*/
#include<stdio.h>
#include<math.h>
#include<malloc.h>

double **a, *b, *x;
int i, j, n;

// protótipos da função
void aloc_var(int m, int n);
void gauss_Seidel(int m, int n);
double somat(int i, int n);

int main()
{
	//void gauss_Seidel(), aloc_var();
	int s, m, n;
	s = 0;
	while(s == 0)
	{
		printf("Resolucao de SELAs - Gauss-Seidel \n");
		printf("\n Forneca as dimensoes da matriz A: m n = ");
		scanf("%d", &m);
		scanf("%d", &n);
		aloc_var(m,n);
		gauss_Seidel(m,n);
		printf("\n Deseja sair do progrma? (1-Sim, 0-Nao)");
		scanf("%d", &s);
	}
}

void gauss_Seidel(int m, int n)
{
	//double somat();
	int k, tol;
	printf("\n Defina a matriz A[%dx%d]\n",m,n);
	for(i=0; i<= m-1; i++)
	{
		for(j=0; j<=n-1; j++)
		{
			printf("\n a[%d,%d]=  ",i+1,j+1);
			scanf("%lf",&a[i][j]);
		}
	}
	printf("\n Defina o vetor B(%d)\n",m);
	for(i=0; i<=m-1; i++)
	{
		printf("\n b[%d]=  ",i+1);
		scanf("%lf",&b[i]);
	}
	printf("\n Defina o vetor x(%d)\n",n);
	for(i=0; i<=n-1; i++)
	{
		printf("\n x[%d]=  ",i+1);
		scanf("%lf",&x[i]);
	}
	printf("\n Defina o numero de iteracoes = ");
	scanf("%d", &tol);
	k = 0;
	while (k < tol)
	{
		for(i=0; i<=n-1; i++)
		{
			x[i] = (b[i] - somat(i,n) + a[i][i]*x[i])/a[i][i];
		}
		k = k+1;
	}
	printf("\n O vetor x eh = \n");
	for (i = 0; i <= n-1; i++)
	{
		printf("%g \n", x[i]);
	}
}

double somat(int i, int n)
{
	double soma;
	soma = 0.0;
	for(j=0; j<=n-1; j++)
	{
		soma = soma + a[i][j]*x[j];
	}
	return soma;
}

void aloc_var(int m, int n)
{
	int i;
	printf("\n Valor de m , n = %d,%d\n",m,n);
	a=(double **) malloc((m+1)*sizeof(double *));
	b=(double *) malloc((m+1)*sizeof(double));
	x=(double *) malloc((n+1)*sizeof(double));
	for(i=0;i < n+1;i++)
	{
		a[i]=(double *)malloc((n+1)*sizeof(double));
	}
}