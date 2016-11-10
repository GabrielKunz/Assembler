#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
//-------------------------------------------------------------Structs-----------------------------------------------------------------
struct instrucao
{
	string m; //mnemonico
	int c; //código
};
struct simbolo
{
	string s; //simbolo
	int e; //endereço
};
//-------------------------------------------------------------Váriaveis Globais:------------------------------------------------------
struct instrucao TabelaInstrucoes[13];
struct simbolo TabelaSimbolos[20];
char linha[80], palavra0[30], palavra1[30], palavra2[30];
int pc = 0, k = 0, erro = 0;
ifstream leitura;
ofstream escrita;
//-------------------------------------------------------------Declaração de funções:--------------------------------------------------
void InicializarTabelaInstrucoes();
void Passo1 (string arg_in, string arg_out);
void Passo2 (string arg_in, string arg_out);
void contar_palavras(string arg_in);
void atualizar_tabela(int contador);
void case_duas_palavras();
void case_tres_palavras();
void imprimir_programa(string arg_in);
void imprimir_tabela_simbolos();
void ler_linha(string arg_in, string arg_out);
void escrever_arquivo(string arg_out, int contador);
string remover_espacos(char *s);
//-------------------------------------------------------------Programa Principal:-----------------------------------------------------
int main(int argc, char const *argv[])
{
	string arg_in;
	string arg_out;
	if(argc>1&&argc<=3){
		arg_in = argv[1];
		if(argc==3){
			arg_out = argv[2];
		}
		else{
			arg_out = "out.asm";
		}
		escrita.open(arg_out.c_str(),ofstream::ios_base::trunc);
		escrita.close();
		InicializarTabelaInstrucoes();
		Passo1(arg_in, arg_out);
		Passo2(arg_in, arg_out);
		return 0;
	}
	else{
		cout<<"Fatal Error: Invalid number of arguments.";
	}
}
//-------------------------------------------------------------Funções:----------------------------------------------------------------

//---------------------------------Inicializar Tabela de Instruções:
void InicializarTabelaInstrucoes()
{
	//inicializar a tabela de instruções
	TabelaInstrucoes[0].m = "ADD";
	TabelaInstrucoes[0].c = 00;
	TabelaInstrucoes[1].m = "BR";
	TabelaInstrucoes[1].c = 01;
	TabelaInstrucoes[2].m = "BRNEG";
	TabelaInstrucoes[2].c = 02;
	TabelaInstrucoes[3].m = "BRPOS";
	TabelaInstrucoes[3].c = 03;
	TabelaInstrucoes[4].m = "BRZERO";
	TabelaInstrucoes[4].c = 04;
	TabelaInstrucoes[5].m = "DIVIDE";
	TabelaInstrucoes[5].c = 05;
	TabelaInstrucoes[6].m = "LOAD";
	TabelaInstrucoes[6].c = 06;
	TabelaInstrucoes[7].m = "MULT";
	TabelaInstrucoes[7].c = 07;
	TabelaInstrucoes[8].m = "READ";
	TabelaInstrucoes[8].c = 8;
	TabelaInstrucoes[9].m = "STOP";
	TabelaInstrucoes[9].c = 9;
	TabelaInstrucoes[10].m = "STORE";
	TabelaInstrucoes[10].c = 10;
	TabelaInstrucoes[11].m = "SUB";
	TabelaInstrucoes[11].c = 11;
	TabelaInstrucoes[12].m = "WRITE";
	TabelaInstrucoes[12].c = 12;
}
//---------------------------------Passo 1:
void Passo1 (string arg_in, string arg_out)
{
	//Construir tabela de simbolos:
	//1.Abrir arquivo;
	//2.Ler linha do arquivo;
	//3.Switch(número de variaveis da linha);
	//4.Case1 : Stop ou End, se for end é fim do passo 1;
	//5.case2 : Colocar simbolo na tabela de simbolos;
	//6.Case3 : Definir (incluir na tabela) o endereço do simbolo;
	contar_palavras(arg_in);
	//Print do programa e da tabela de símbolos:
	for (int x = 0; x < 20; ++x)
	{
		if (TabelaSimbolos[x].e == -1)
		{
			erro = 1;
			cout<<"\n\nError: Variable not declared. A empty "<<arg_out<<" file was generated\n"<<endl;
		}
	}
	if (erro == 0)
	{
		//Print do programa
		imprimir_programa(arg_in);
		//Print da tabela de simbolos
		imprimir_tabela_simbolos();
	}
}
//---------------------------------Passo 2:
void Passo2 (string arg_in, string arg_out)
{
	//Construir arquivo do programa:
	//1.Inicializar e criar arquivo;
	//2.Abrir programa;
	//3.Ler linha;
	//4.Escrever no novo arquivo os comandos de acordo com a tabela de instruções e de símbolos;
	if (erro == 0)
	{
		cout<<"\n\n\tPrograma Gerado:\n"<<endl;
		ler_linha(arg_in, arg_out);
	}	
}
//---------------------------------Contar Palavras:
void contar_palavras(string arg_in)
{
	int i = 0, j=0, contador=0;
	//i - contador para linha, j - contador para palavra, k - contador para tabela de simbolos
	//pc - contador de linhas do programa;
	leitura.open(arg_in.c_str());
	while(!leitura.eof()){
		leitura.getline(linha,80);
		//zerar palavras e contadores
		for (j = 0; j < 30; j++)
		{
			palavra0[j]=' ';
			palavra1[j]=' ';
			palavra2[j]=' ';
		}
		//cout<<linha<<"\n"; -- printar linha lida
		i=0;
		j=0;
		contador=0;
		//pegar palavra 0
		while(linha[i]==' '){
			i++;
		}
		while(linha[i]!=' '&&linha[i]!='\0'){
			palavra0[j] = linha[i];
			i++;
			j++;
		}
		contador++;
		//pegar palavras 1 e 2
		if(linha[i]!='\0'){
			//pegar palavra 1
			while(linha[i]==' '){
				i++;
			}
			j=0;
			while(linha[i]!=' '&&linha[i]!='\0'){
				palavra1[j] = linha[i];
				i++;
				j++;
			}
			contador++;
			//pegar palavra 2
			if(linha[i]!='\0'){
				while(linha[i]==' '){
					i++;
				}
				j=0;
				while(linha[i]!=' '&&linha[i]!='\0'){
					palavra2[j] = linha[i];
					i++;
					j++;
				}
				//verificar se o numero de palavras é maior que 3
				while(linha[i]!='\0'){
					if (linha[i]!='\0'&&linha[i]!=' ')
					{
						contador++;
					}
					i++;
				}
				contador++;
			}	
		}
		atualizar_tabela(contador);
	}
	leitura.close();
}
//---------------------------------Atualizar Tabela:
void atualizar_tabela(int contador)
{
	switch(contador){
		case 1:
			break;
		case 2:
			case_duas_palavras();
			break;
		case 3:
			case_tres_palavras();
			break;
		default:
			cout<<"Fatal Error: Invalid number of words in line:  "<<pc<<"\n\n";
			break;
	}
	pc++;
}
//---------------------------------Case Duas Palavras:
void case_duas_palavras()
{
	bool palavraexistente;
	for (int i = 0; i < 20; i++)
			{
				if (TabelaSimbolos[i].s==string(palavra1))
				{
					palavraexistente = true;
				}
			}
			if (!palavraexistente)
			{
				TabelaSimbolos[k].s = string(palavra1);
				TabelaSimbolos[k].e = -1;
				k++;
			}
}
//---------------------------------Case Tres Palvras:
void case_tres_palavras()
{
	int space;
	bool encontrou_variavel = false;
	string variavel_warning;
	for (int i = 0; i < 20; i++)
	{
		if (TabelaSimbolos[i].s==string(palavra0)){
			//verifico o tamanho reservado para a variavel
			space = atoi(palavra2);
			if (space>0){
				TabelaSimbolos[i].e = pc;
				pc = pc + space - 1;
			}
			else{
				cout<<"Fatal Error - Variable Scpace is equal to 0 or negative number"<<endl;
			}
			encontrou_variavel = true;
		}
	}
	if (!encontrou_variavel)
	{
		variavel_warning = remover_espacos(palavra0);
		cout<<"\n\nWarning: Variable '"<<variavel_warning<<"' not used in this program\n";
	}

}
//---------------------------------Imprimir Programa:
void imprimir_programa(string arg_in)
{
	leitura.open(arg_in.c_str());
	printf("\n\tFim do Passo 1: ");
	printf("\n\n\tPrograma:\n\n");
	while(!leitura.eof()){
		leitura.getline(linha,80);
		cout<<linha<<"\n";
	}
	leitura.close();
}
//---------------------------------Imprimir Tabela de Simbolos:
void imprimir_tabela_simbolos()
{
	cout<<"\n\n\tTabela de Símbolos:\n\n"<<endl;
	for (int i = 0; i < 20; i++)
	{
		cout<<"Símbolo: "<<TabelaSimbolos[i].s<<"\tEndereço: "<<TabelaSimbolos[i].e<<"\n";
	}
}
//---------------------------------Ler linha para o passo 2:
void ler_linha(string arg_in, string arg_out)
{
	int i = 0, j=0, contador=0;
	leitura.open(arg_in.c_str());
	while(!leitura.eof()){
		leitura.getline(linha,80);
		//zerar palavras e contadores
		for (j = 0; j < 30; j++)
		{
			palavra0[j]=' ';
			palavra1[j]=' ';
			palavra2[j]=' ';
		}
		//cout<<linha<<"\n"; -- printar linha lida
		i=0;
		j=0;
		contador=0;
		//pegar palavra 0
		while(linha[i]==' '){
			i++;
		}
		while(linha[i]!=' '&&linha[i]!='\0'){
			palavra0[j] = linha[i];
			i++;
			j++;
		}
		contador++;
		//pegar palavras 1 e 2
		if(linha[i]!='\0'){
			//pegar palavra 1
			while(linha[i]==' '){
				i++;
			}
			j=0;
			while(linha[i]!=' '&&linha[i]!='\0'){
				palavra1[j] = linha[i];
				i++;
				j++;
			}
			contador++;
			//pegar palavra 2
			if(linha[i]!='\0'){
				while(linha[i]==' '){
					i++;
				}
				j=0;
				while(linha[i]!=' '&&linha[i]!='\0'){
					palavra2[j] = linha[i];
					i++;
					j++;
				}
				contador++;
			}	
		}
		escrever_arquivo(arg_out, contador);
	}
	leitura.close();
}
//---------------------------------Escrever linha do passo 2 no arquivo:
void escrever_arquivo(string arg_out, int contador)
{
	ostringstream instrucao, variavel;
	string palavra;
	switch (contador){
	case 1:
		for (int i = 0; i < 13; ++i)
		{
			palavra = remover_espacos(palavra0);
			if (TabelaInstrucoes[i].m == palavra)
			{
				instrucao<<(TabelaInstrucoes[i].c);
				escrita.open(arg_out.c_str(), ofstream::ios_base::app);
				escrita<<instrucao.str()<<endl;
				escrita.close();
				cout<<instrucao.str()<<endl;
			}
		}
		break;
	case 2:
		for (int i = 0; i < 13; ++i)
		{
			palavra = remover_espacos(palavra0);
			if (TabelaInstrucoes[i].m == palavra)
			{
				instrucao<<(TabelaInstrucoes[i].c);
				for (int i = 0; i < 20; ++i)
				{
					if (TabelaSimbolos[i].s == string(palavra1))
					{
						variavel<<(TabelaSimbolos[i].e);
						escrita.open(arg_out.c_str(), ofstream::ios_base::app);
						escrita<<instrucao.str()<<" "<<variavel.str()<<endl;
						escrita.close();
						cout<<instrucao.str()<<" "<<variavel.str()<<endl;
					}
				}
			}
		}
		break;
	case 3:
		break;
	default:
		cout<<"Fatal Error - Invalid number of words in on line (max 3)"<<endl;
	}
}
//---------------------------------Remover espaços ao trasnformar um vetor char para string:
string remover_espacos(char *s)
{
	char* cpy = s;  // an alias to iterate through s without moving s
    char* temp = s;
    while (*cpy)
    {
        if (*cpy != ' ')
            *temp++ = *cpy;
        cpy++;
    }
    *temp = 0;

    return s;
}