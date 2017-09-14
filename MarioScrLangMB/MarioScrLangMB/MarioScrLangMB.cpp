#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include "parsing.h"


void wypiszZmienne(struct ListaZmiennych * listaZmiennych) {
	while (listaZmiennych != NULL) {
		wypiszZmienna(*(listaZmiennych->zmienna));
		listaZmiennych = listaZmiennych->nastepna;
	}
}

void freeListaZmiennych(struct ListaZmiennych ** listaZmiennych) {
	if ((*listaZmiennych) != NULL) {
		freeListaZmiennych(&((*listaZmiennych)->nastepna));
		free((*listaZmiennych));
		(*listaZmiennych) = NULL;
	}
}

int main()
{
	int liczbaOperacji = 0, iloscOperacji=0;
	int liczbaZnakow = LINE_SIZE;
	int wierszPos = 0;
	struct ListaZmiennych *listaZmiennych = NULL;
	//struct Token*  token = (struct Token*)malloc(sizeof(struct Token));

	struct TabelaZmiennych tabelaZmiennych;
	char wiersz[LINE_SIZE];
	initTabelaZmiennych(&tabelaZmiennych);

	scanf("%i\n", &liczbaOperacji);
	fgets(wiersz, liczbaZnakow, stdin );
	czytajZmienne(&tabelaZmiennych, wiersz, &listaZmiennych);

	parsuj(&tabelaZmiennych, liczbaOperacji, &iloscOperacji);
	printf("%i\n", iloscOperacji);
	wypiszZmienne(listaZmiennych);

	freeListaZmiennych(&listaZmiennych);
	freeTabelaZmiennych(&tabelaZmiennych);

	return 0;
}

