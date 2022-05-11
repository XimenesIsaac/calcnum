#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*============================== Conversor =======================================*/

/* Converte um numero da base 10 para a base desejada.
 *
 * Entrada: numero positivo e a base desejada.
 * Saida: void
 */
void conversorBase(double number, int base)
{
	long int n = number, aux;
	double f = number - n;
	int c[128];
	int i;

	i = 0;
	while(n >= base) {
		aux = n / base;
		c[i++] = n - aux * base;
		n = aux;
	}
	c[i] = n;

	while(i >= 0) {
		if(c[i] < 10)
			printf("%d", c[i--]);
		else
			printf("%c", c[i--]-10+'a');
	}
	printf(".");

	/*for(i=0; f!=0; i++) {*/
	for(i=0; i<10; i++) {
		f = f * base;
		c[i] = f;
		f = f - c[i];
		if(c[i] < 10)
			printf("%d", c[i]);
		else
			printf("%c", c[i]-10+'a');
	}
}

/* Le um numero e o imprime nas bases 2, 8 e 16.
 *
 * Entrada: void
 * Saida: 0, caso tenha executado corretamente; 1, em caso de erro.
 */
int conversao()
{
	double number;
	int base;

	printf("Digite o numero a ser convertido: ");
	scanf("%lf", &number);

	if(number < 0)
		return 1;

	base = 2;
	printf("\nbase %2d = ", base);
	conversorBase(number, base);
	base = 8;
	printf("\nbase %2d = ", base);
	conversorBase(number, base);
	base = 16;
	printf("\nbase %2d = ", base);
	conversorBase(number, base);

	return 0;
}

/*================================== Sistemas Lineares =====================================*/

/* Desaloca um vetor previamente alocado.
 *
 * Entrada: endereco de um vetor do tipo double.
 * Saida: void
 */
void desalocaVetor(double **v) {
	free(*v);
	*v = NULL;
}

/* Aloca um vetor.
 *
 * Entrada: endereco de um ponteiro do tipo double; tamanho do vetor.
 * Saida: 0, caso tenha alocado o vetor; 1, caso contrario.
 */
int alocaVetor(double **v, int size) {
	*v = malloc(sizeof(double)*size);
	if(*v == NULL)
		return 1;
	return 0;
}

/* Desaloca uma matriz.
 *
 * Entrada: endereco de um ponteiro para uma matriz do tipo double; quantidade de linhas da matriz.
 * Saida: void.
 */
void desalocaMatriz(double ***m, int size) {
	int i;

	for(i=0; i<size; i++)
		free((*m)[i]);
	free(*m);
	*m = NULL;
}

/* Aloca uma matriz.
 *
 * Entrada: endereco de um ponteiro para uma matriz do tipo double; quantidade de linhas e colunas da matriz.
 * Saida: 0, caso tenha alocado a matriz; 1, caso contrario.
 */
int alocaMatriz(double ***m, int sizei, int sizej) {
	int i;

	*m = malloc(sizeof(double)*sizei);
	if(*m == NULL)
		return 1;

	for(i=0; i<sizei; i++) {
		(*m)[i] = malloc(sizeof(double)*sizej);
		if((*m)[i] == NULL) {
			desalocaMatriz(m, i);
			return 1;
		}
	}

	return 0;
}

/* Troca duas linhas de uma matriz.
 *
 * Entrada: uma matriz do tipo double; numero das duas linhas a serem trocadas.
 * Saida: void.
 */
void trocaLinhaMatriz(double **m, int i1, int i2)
{
	double *aux = m[i1];
	m[i1] = m[i2];
	m[i2] = aux;
}

/* Aplica o Metodo de Gauss em uma matriz aumentada.
 *
 * Entrada: matriz aumentada do tipo double; tamanho da matriz.
 * Saida: void.
 */
void Gauss(double **m, int size) {
	double multiplicador;
	int i, j, k;

	for(i=0; i<size; i++) {
		for(k=i+1; m[i][i]==0 && k<size; k++) { /*Se o indice de m[i][i] for zero, procura uma linha abaixo para trocar com a atual. */
			if(m[k][i] != 0)
				trocaLinhaMatriz(m, i, k);
		}
		if(m[i][i] == 0) /*Se o indice de m[i][i] continuar 0, indica que esta interacao terminou.*/
			continue;

		for(k=i+1; k<size; k++) {
			multiplicador = -m[k][i] / m[i][i];
			for(j=0; j<=size; j++) {
				m[k][j] = m[k][j] + m[i][j] * multiplicador;
			}
		}
	}
}

/*
 * Encontra o valor de uma solucao para a um sistema linear, caso exista,
 * aplicando a solucao retroativa em uma matriz triangular inferior.
 *
 * Entrada: matriz aumentada do tipo double; tamanho da matriz;
 * endereco do vetor para armazenar a solucao do sistema.
 * Saida: -1, caso o sistema seja impossivel;
 * 0, caso seja possivel indeterminado; 1, caso possivel e determinado.
 */
int solucaoRetroativa(double **m, int size, double **x) {
	int i, j;
	double somatorio;

	for(i=size-1; i>=0; i--) {
		somatorio = 0;
		for(j=i+1; j<size; j++) {
			somatorio = somatorio + m[i][j] * (*x)[j];
		}
		if(m[i][i] != 0) {
			(*x)[i] = (m[i][size] - somatorio) / m[i][i];
		} else {
			if(m[i][size] == somatorio) {
				(*x)[i] = 0;
			} else {
				return -1;
			}
		}
	}

	return 0;
}

/* Le um arquivo contendo um sistema linear e imprime uma solucao para o sistema, caso exista.
 *
 * Entrada: void
 * Saida: 0, caso tenha executado corretamente; 1, em caso de erro de alocacao de memoria;
 * 2, em caso de falha ao acessar o arquivo; 3, em caso de problema no layout do arquivo.
 */


int sistemaLinear() {
	char path[200] = "";
	FILE *fp;

	double **m, *x;
	int size;
	int i, j;
	double aux;

	printf("Digite o caminho para o arquivo: ");
	scanf("%s", path);

	fp = fopen(path, "r");
	if(fp == NULL)
		return 2;
	fscanf(fp, "%d", &size);

	if(alocaMatriz(&m, size, size+1) != 0) {
		fclose(fp);
		return 1;
	}

	for(i=0; i<size; i++) {
		for(j=0; j<=size; j++) {
			if(feof(fp)) {
				desalocaMatriz(&m, size);
				return 3;
			}
			fscanf(fp, "%lf", &aux);
			m[i][j] = aux;
		}
	}

	fclose(fp);

	Gauss(m, size);

	for(i=0; i<size; i++)
	{
		for(j=0; j<(size+1); j++)
		{
			printf("%12.6f ", m[i][j]);
		}
		printf("\n");
	}

	if(alocaVetor(&x, size) != 0) {
		desalocaMatriz(&m, size);
		return 1;
	}

	switch(solucaoRetroativa(m, size, &x)) {
	case -1:
		printf("Sistema impossivel.\n");
		break;
	case 0:
		printf("Sistema possivel e indeterminado.\n");
		printf("Uma solucao para o sistema:\n");
		for(i=0; i<size; i++)
			printf("X%-2d = %12.6f\n", i+1, x[i]);
		break;
	case 1:
		printf("Sistema possivel e determinado.\n");
		printf("Solucao do sistema:\n");
		for(i=0; i<size; i++)
			printf("X%-2d = %12.6f\n", i+1, x[i]);
		break;
	}

	desalocaMatriz(&m, size);
	desalocaVetor(&x);

	return 0;
}



/*=============================== Equacoes ==================================*/

typedef struct _intervalo {
	double min, max;
} intervalo;

/* Desaloca um vetor previamente alocado.
 *
 * Entrada: endereco de um vetor do tipo int.
 * Saida: void
 */
void desalocaVetorEq(int **v) {
	free(*v);
	*v = NULL;
}

/* Aloca um vetor.
 *
 * Entrada: endereco de um ponteiro do tipo int; tamanho do vetor.
 * Saida: 0, caso tenha alocado o vetor; 1, caso contrario.
 */
int alocaVetorEq(int **v, int size) {
	*v = malloc(sizeof(int)*size);
	if(*v == NULL)
		return 1;
	return 0;
}

/* Aplica o teorema de Lagrange em uma equacao algebrica.
 *
 * Entrada: grau da equacao, vetor de coeficientes da equacao.
 * Saida: resultado do teorema de Lagrange para a equacao.
 */
double teoremaLagrange(int n, int a[]) {
	int k, B;
	int i;
	double result, aux1, aux2;

	for(i=n, k=-1, B=0; i>=0; i--) {
		if(a[i] < 0) {
			if(k == -1)
				k = i;
			if(abs(a[i]) > B)
				B = abs(a[i]);
		}
	}

	aux1 = (double)B / (double)a[n];
	aux2 = 1.0 / (n-k);

	result = 1.0 + pow(aux1, aux2);

	return result;
}

/* Calcula os limitantes para as raizes positivas e negativas de uma equacao algebrica.
 *
 * Entrada: grau da equacao, vetor de coeficientes da equacao,
 * intervalo das raizes positivas e das negativas.
 * Saida: 0, caso tenha executado corretamente; 1, em caso de erro.
 */
int calculoLimitante(int n, int a[], intervalo *negativas, intervalo *positivas) {
	int *p1, *p2, *p3;
	int i;

	p1 = p2 = p3 = NULL;

	if(alocaVetorEq(&p1, n+1) != 0)
		return 1;
	if(alocaVetorEq(&p2, n+1) != 0) {
		desalocaVetorEq(&p1);
		return 1;
	}
	if(alocaVetorEq(&p3, n+1) != 0) {
		desalocaVetorEq(&p1);
		desalocaVetorEq(&p2);
		return 1;
	}

	for(i=0; i<=n; i++) {
		p1[i] = a[n-i];
	}

	positivas->max = teoremaLagrange(n, a);
	positivas->min = 1 / teoremaLagrange(n, p1);

	for(i=0; i<=n; i++) {
		if(i%2 == 0)
			p2[i] = a[i];
		else
			p2[i] = -a[i];
	}
	for(i=0; i<=n; i++) {
		if(i%2 == 0)
			p3[i] = a[n-i];
		else
			p3[i] = -a[n-i];
	}

	negativas->max = -(1 / teoremaLagrange(n, p3));
	negativas->min = -teoremaLagrange(n, p2);

	desalocaVetorEq(&p1);
	desalocaVetorEq(&p2);
	desalocaVetorEq(&p3);

	return 0;
}

/* Calcula a integral da equacao algebrica.
 *
 * Entrada: grau da equacao, vetor de coeficientes da equacao,
 * ponteiro para o vetor de coeficientes da equacao derivada.
 * Saida: void.
 */
void integral(int n, int a[], int *d[]) {
	int i;

	for(i=n; i>0; i--) {
		(*d)[i-1] = a[i]*i;
	}
}

/* Calcula o resultado da equacao algebrica para um valor x.
 *
 * Entrada: grau da equacao, vetor de coeficientes da equacao, valor de x.
 * Saida: resultado da equacao.
 */
double valorEq(int n, int a[], double x) {
	int i;
	double value;

	for(i=n, value=0; i>=0; i--) {
		value = value + a[i]*pow(x,i);
	}

	return value;
}

/* Calcula o modulo de um numero do tipo double.
 *
 * Entrada: numero tipo double.
 * Saida: numero em modulo.
 */
double calcMod(double a) {
	if(a < 0)
		return -a;
	return a;
}

/* Verifica se a equacao algebrica eh nula.
 *
 * Entrada: grau da equacao, vetor de coeficientes.
 * Saida: 0, se a equacao nao for nula; 1, caso contrario.
 */
int eqNula(int n, int a[]) {
	int i;

	for(i=0; i<=n; i++) {
		if(a[i] != 0)
			return 0;
	}
	return 1;
}

/* Calcula uma aproximacao da raiz de uma equacao algebrica utilizando o Metodo de Newton.
 *
 * Entrada: grau da equacao, vetor de coeficientes da equacao, intervalo,
 * ponteiro para o resultado da aproximacao, precisao.
 * Saida: 0, caso tenha executado corretamente; 1, em caso de erro de alocacao de memoria;
 * 2, no caso de o Metodo de Newton nao convergir.
 */
int metodoNewton(int n, int a[], double *x, double precisao) {
	int *d;
	double variacao;
	double aux;
	int counter;

	if(alocaVetorEq(&d, n) != 0)
		return 1;

	integral(n, a, &d);

/*	if(eqNula(n, a) || eqNula(n-1, d)) { */ /*Verificar se alguma equacao eh nula*/
/*		desalocaVetorEq(&d);
		return 2;
	}
	if((valorEq(n, a, *x) * valorEq(n-1, d, *x)) < 0) { */ /*Verificar se f(x0)*f'(x0) < 0*/
/*		desalocaVetorEq(&d);
		return 2;
	}*/

	counter = 0;
	do {
		aux = *x - (valorEq(n, a, *x) / valorEq((n-1), d, *x));
		variacao = calcMod(aux - *x);
		*x = aux;
		counter++;
	} while(variacao >= precisao && counter <= 1000);

	desalocaVetorEq(&d);

	return 0;
}

/* Calcula os limitantes das raizes e uma aproximacao da raiz de uma equacao algebrica.
 *
 * Entrada: void.
 * Saida: 0, caso tenha executado corretamente; 1, em caso de erro de alocacao de memoria;
 * 2, se An < 0; 3, se A0 = 0; 4, se o Metodo de Newton divergir.
 */
int eqAlgebrica() {
	int *a;
	int n, i;
	intervalo negativas, positivas;
	double precisao = 1.0 / pow(10, 6);/* Precisao de 10^-6.*/
	double x;
	int erro;

	printf("Digite o grau da equacao: ");
	scanf("%d", &n);

	if(alocaVetorEq(&a, n+1) != 0)
		return 1;

	printf("\nDigite os coeficientes da equacao:\n");
	for(i=n; i>=0; i--) {
		printf("a[%d] = ", i);
		scanf("%d", &a[i]);
	}
	printf("\n");

	if(a[n] < 0) {
		desalocaVetorEq(&a);
		return 2;
	}
	if(a[0] == 0) {
		desalocaVetorEq(&a);
		return 3;
	}

	if(calculoLimitante(n, a, &negativas, &positivas) != 0) {
		desalocaVetorEq(&a);
		return 1;
	}

	printf("%f <= E+ <= %f\n", positivas.min, positivas.max);
	printf("%f <= E- <= %f\n", negativas.min, negativas.max);

	/**x = (interv.max - interv.min) / 2;*//*Calculo de x0*/
	x = positivas.max;

	erro = metodoNewton(n, a, &x, precisao);

	desalocaVetorEq(&a);

	switch(erro) {
	case 1:
		return 1;
	case 2:
		return 4;
	}

	printf("Raiz aproximada: %.6f\n", x);

	return 0;
}

/*=================================== Programa ====================================*/

int main(void) {
	char opt;
	char read[10];
	int error;

	do {
		printf("C - Para conversao de bases (de decimal para binario, octal e hexadecimal\n");
		printf("S - Sistemas Lineares\n");
		printf("E - Equacoes Algebricas\n");
		printf("F - Finalizar\n");

		printf("\n Opcao: ");
		scanf("%s", read);
		opt = read[0];

		switch(opt) {
		case 'C':
			error = conversao();
			if(error == 1) {
				printf("\nERRO: digite um numero maior ou igual a zero.\n");
			}
			break;
		case 'S':
			error = sistemaLinear();
			switch(error) {
			case 1:
				printf("\nERRO: memoria insuficiente.\n");
				break;
			case 2:
				printf("\nERRO: caminho para arquivo invalido.\n");
				break;
			case 3:
				printf("\nERRO: problema com o layout do arquivo.\n");
				break;
			}
			break;
		case 'E':
			error = eqAlgebrica();
			switch(error) {
			case 1:
				printf("\nERRO: memoria insuficiente.\n");
				break;
			case 2:
				printf("Erro: An < 0.\n");
				break;
			case 3:
				printf("Erro: A0 = 0.\n");
				break;
			case 4:
				printf("Erro: O Metodo de Newton nao converge no intervalo.\n");
				break;
			}
			break;
		case 'F':
			break;
		default:
			printf("\nERRO: opcao invalida.\n");
			break;
		}
		printf("\n\n");
	} while(opt != 'F' || 'f');

	return EXIT_SUCCESS;
}
