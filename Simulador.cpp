#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;
// Definição das instruções
#define ADD 00
#define BR 01
#define BRNEG 02
#define BRPOS 03
#define BRZERO 04
#define DIVIDE 05
#define LOAD 06
#define MULT 07
#define READ 8 //Só funciona se a constante for 8 ao inves de 08
#define STOP 9 //Só funciona se a constante for 9 ao inves de 09
#define STORE 10
#define SUB 11
#define WRITE 12

//Declaração de variaveis globais

//Estrutura de cada parte do vetor memoria
struct memoria {
	int op; //operador
	int opr; //operando
};

//Vetor memoria
struct memoria mem[512];

//pc, acumulador , r0 e r1
int pc = 0, ac, r0, r1;


//Declaração de funções
void rodar_programa();

//************************************Programa principal
int main(int argc, char const *argv[])
{
	//declaração de variaveis
	FILE * f ;
  	int j=0, erro = 0;
  	char imprimir;
  	char nomeArq [30];
  	string arg_in;

  	if (argc>1&&argc<=2)
  	{
  		arg_in = argv[1];
  	}
  	else
  	{
  		erro = 1;
  	}
  	if (erro == 0)
  	{
  		//ler arquivo (programa) e salva na memoria
	    f = fopen (arg_in.c_str(), "r") ;
	    while (!feof(f)) {
	        fscanf (f, "%d%d", &mem[pc].op, &mem[pc].opr);
	        pc++;
		}

		//imprimir o programa original na tela
		printf("\n\n\tEste é o programa que sera simulado:\n\n");
		while(pc!=0)	{
			printf("%d %d;\n", mem[j].op, mem[j].opr);
			j++;
			pc--;
		}
		pc = 0;

		//simulador (switch case)
		rodar_programa();

		//imprimir memoria após a execução do programa
		do
		{
			getchar();
			printf("\n\nDeseja imprimir a memoria? (s ou n): ");
			scanf("%c", &imprimir);
		}
		while (imprimir != 's' && imprimir != 'n');

		if(imprimir == 's'){
			pc = 0;
			printf("\n\n\tMemoria apos a execucao do programa:\n\n");
			while(pc<512){
				printf("%d : %d %d\n", pc, mem[pc].op, mem[pc].opr);
				pc++;
			}
		}
		return 0;
	}
	else
	{
		printf("Erro fatal! Nao foi possivel executar o programa\n");
	}
}
//************************************Fim do programa principal

//Funções
void rodar_programa()
{
	while(mem[pc].op!=STOP){
		switch(mem[pc].op){
		case ADD :
			ac = ac + mem[mem[pc].opr].op;
			pc++;
			break;
		case BR:
			pc = mem[pc].opr;
			break;
		case BRNEG:
			if (ac<0){
				pc = mem[pc].opr;
			}
			else{
				pc++;
			}
			break;
		case BRPOS:
			if (ac>0){
				pc = mem[pc].opr;
			}
			else{
				pc++;
			}
			break;
		case BRZERO:
			if (ac==0){
				pc = mem[pc].opr;
			}
			else{
				pc++;
			}
			break;
		case DIVIDE:
			ac = ac/mem[mem[pc].opr].op;
			pc++;
			break;
		case LOAD:
			ac = mem[mem[pc].opr].op;
			pc++;
			break;
		case MULT:
			ac = ac*mem[mem[pc].opr].op;
			break;
		case READ:
			printf("\n\nDigite um numero : ");
			scanf("%d",&mem[mem[pc].opr].op);
			pc++;
			break;
		case STOP:
			//nao faz nada
			break;
		case STORE:
			mem[mem[pc].opr].op = ac;
			pc++;
			break;
		case SUB:
			ac = ac - mem[mem[pc].opr].op;
			pc++;
			break;
		case WRITE:
			printf("\n\nValor do endereco %d : %d\n", mem[pc].opr, mem[mem[pc].opr].op);
			pc++;
			break;
		default:
			mem[pc].op = STOP;
			break;											
		}
	}
}