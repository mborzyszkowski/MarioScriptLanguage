#ifndef TOKENY_H
#define TOKENY_H

#include "tablicaHashowa.h"

#define MAKSYMALNY_ROZMIAR_LICZBY 32
#define LINE_SIZE 1100


#define TOKEN_TYP_LICZBA 0
#define TOKEN_TYP_ZMIENNA 1
#define TOKEN_TYP_OPERACJA 2

#define ARNOSC_ZERO  0
#define ARNOSC_JEDEN 1
#define ARNOSC_DWA   2

#define OP_NOOP   -1
#define OP_ASSIGN 0
#define OP_OR     1
#define OP_AND    2
#define OP_NEQ    3
#define OP_EQ     4
#define OP_LT     5
#define OP_GT     6
#define OP_LE     7
#define OP_GE     8
#define OP_PLUS   9
#define OP_MINUS  10
#define OP_DIV    11
#define OP_MULT   12
#define OP_MOD    13
#define OP_NEG    14
// --- ( 
#define OP_LPAR1  15 
// --- ) 
#define OP_RPAR1  16 
// --- { 
#define OP_LPAR2  17 
// --- } 
#define OP_RPAR2  18
#define OP_WARUNEK 19
#define OP_WHILE  20

#define PRIORYTET_0 0
#define PRIORYTET_1 1
#define PRIORYTET_2 2
#define PRIORYTET_3 3
#define PRIORYTET_4 4
#define PRIORYTET_5 5
#define PRIORYTET_6 6
#define PRIORYTET_7 7
#define PRIORYTET_8 8
#define PRIORYTET_9 9

struct Token {
	int typ;
	int arnosc;
	struct Zmienna* zmienna;
	int intValue;
	bool isEmpty;
	int operacja;
	int priorytet;
};

struct ListaZmiennych {
	struct Zmienna *zmienna;
	struct ListaZmiennych *nastepna;
};

void dodajZmiennaDoListy(struct ListaZmiennych ** listaZmiennych, struct Zmienna *zmienna);
void wypiszListeZmiennych(struct ListaZmiennych * listaZmiennych);
void czytajZmienne(struct TabelaZmiennych* tabelaZmiennych, char* wiersz, struct ListaZmiennych ** listaZmiennych);
int czytajLiczbe(char* wiersz, int *startPos);
struct Zmienna* czytajZmienna(struct TabelaZmiennych* tabelaZmiennych, char* wiersz, int *startPos);
int nastepnyTokienZWiersza(struct TabelaZmiennych* tabelaZmiennych, char* wiersz, int *startPos, struct Token** tokenResult);
int nastepnyTokien(struct TabelaZmiennych* tabelaZmiennych, char* wiersz, int *startPos, struct Token** tokenResult);
struct Token* kopiaTokenu(struct Token* token);
void freeToken(struct Token* token);

#endif // TOKENY_H
