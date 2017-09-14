#ifndef TABICAHASHOWA_H
#define TABICAHASHOWA_H

#include "zmienna.h"

#define ROZMIAR_TABLICY_HASHOWEJ 3000

struct ElementTablicy {
	struct Zmienna* zmienna;
	struct ElementTablicy* nastepny;
};

struct TabelaZmiennych {
	struct ElementTablicy** elemTab;
};

bool myStrEq(char* str1, char* str2);
void initTabelaZmiennych(struct TabelaZmiennych* tabelaZmiennych);
void freeTabelaZmiennych(struct TabelaZmiennych* tabelaZmiennych);
int funkcjaHash(char* nazwa);
void dodajDoTablicy(struct TabelaZmiennych* tabelaZmiennych, struct Zmienna* zmienna);
struct Zmienna* znajdzWTablicy(struct TabelaZmiennych tabelaZmiennych, char* nazwa);
struct Zmienna* znajdzWTablicy(struct TabelaZmiennych tabelaZmiennych, char* nazwa);
struct Zmienna* znajdzWTablicy(struct TabelaZmiennych tabelaZmiennych, char* nazwa);
void printTablica(struct TabelaZmiennych tabelaZmiennych);

#endif // TABICAHASHOWA_H

