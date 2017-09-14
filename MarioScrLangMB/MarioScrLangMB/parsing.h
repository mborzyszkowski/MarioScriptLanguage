#ifndef PARSING_H
#define PARSING_H

#include "tablicaTokenow.h"
#include "evaluate.h"

#define PROGR_WYR   0
#define PROGR_IF    1
#define PROGR_WHILE 2
#define PROGR_COMP  3

struct ProgramTree {
	int typWezla;
	struct TablicaTokenow* warunek;
	struct ProgramTree* komenda;
	struct ProgramTree* nastepepnaKomenda;
};

struct ProgramTree* czytajKomende(struct TablicaTokenow** tokenTab);
struct ProgramTree* czytajListeKomend(struct TablicaTokenow** tokenTab, bool inCommand);
int nastepnyTokienNieZero(struct TabelaZmiennych* tabelaZmiennych, char* wiersz, int *wierszPos, struct Token** token);
void intTablicaTokenow(struct TabelaZmiennych* tabelaZmiennych, struct TablicaTokenow** firstReadToken, struct TablicaTokenow** lastReadToken);
void stacksEvaluation(struct TablicaTokenow** outputStack, struct TablicaTokenow** operationStack, bool doLewegoNawiasu, int limitOperacji, int *liczbaOperacji);
struct TablicaTokenow* czytajWyrazenie(struct TablicaTokenow** tokenTab, int *wenPar, bool inCond);
struct ProgramTree* czytajKomende(struct TablicaTokenow** tablicaTokenow);
struct ProgramTree* czytajListeKomend(struct TablicaTokenow** tokenTab, bool inCommand);
struct Token* wyliczWyrazenie(struct TablicaTokenow* wyr, struct TablicaTokenow** outputStack, struct TablicaTokenow** operationStack, int limitOperacji, int *liczbaOperacji);
void wykonajProgram(struct ProgramTree** program, struct TablicaTokenow** outputStack, struct TablicaTokenow** operationStack, int limitOperacji, int *liczbaOperacji);
void parsuj(struct TabelaZmiennych* tabelaZmiennych, int limitOperacji, int *liczbaOperacji);

#endif // PARSING_H
