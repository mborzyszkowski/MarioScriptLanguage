#include "tablicaHashowa.h"

bool myStrEq(char* napis1, char* napis2) {
	char *pom1 = napis1, *pom2 = napis2;

	while (*pom1 != '\0') {
		if (*pom2 == '\0' || *pom2 > *pom1 || *pom2 < *pom1) return false;
		pom1++;
		pom2++;
	}
	if (*pom2 != '\0') return false;
	return true;
}

void initTabelaZmiennych(struct TabelaZmiennych* tabelaZmiennych) {
	tabelaZmiennych->elemTab = (struct ElementTablicy**)malloc(sizeof(struct ElementTablicy*)*ROZMIAR_TABLICY_HASHOWEJ);
	for (int i = 0; i < ROZMIAR_TABLICY_HASHOWEJ; i++) {
		tabelaZmiennych->elemTab[i] = NULL;
	}
}


void freeTabelaZmiennych(struct TabelaZmiennych* tabelaZmiennych) {
	for (int i = 0; i < ROZMIAR_TABLICY_HASHOWEJ; i++) {
		if (tabelaZmiennych->elemTab[i] != NULL) {
			free(tabelaZmiennych->elemTab[i]);
			tabelaZmiennych->elemTab[i] = NULL;
		}
	}
}

int funkcjaHash(char* nazwa) {
	int res = 0;
	char* pom = nazwa;
	while (*pom != '\0') {
		res = 31 * res + *pom + 65;
		pom++;
	}
	return res % ROZMIAR_TABLICY_HASHOWEJ;
}

void dodajDoTablicy(struct TabelaZmiennych* tabelaZmiennych, struct Zmienna* zmienna) {
	int hash = funkcjaHash(zmienna->nazwa);
	ElementTablicy* elTab = tabelaZmiennych->elemTab[hash];
	ElementTablicy* pom;
	bool dodane = false;
	while (elTab != NULL && !dodane) {
		if (myStrEq(zmienna->nazwa, elTab->zmienna->nazwa)) dodane = true;
		else {
			if (elTab->nastepny == NULL) {
				pom = (ElementTablicy*)malloc(sizeof(ElementTablicy*));
				pom->zmienna = zmienna;
				pom->nastepny = NULL;
				elTab->nastepny = pom;
				dodane = true;
			}
			else {
				elTab = elTab->nastepny;
			}
		}
	}
	if (elTab == NULL && !dodane) {
		pom = (ElementTablicy*)malloc(sizeof(ElementTablicy));
		pom->zmienna = zmienna;
		pom->nastepny = NULL;
		tabelaZmiennych->elemTab[hash] = pom;
		dodane = true;
	}
}

struct Zmienna* znajdzWTablicy(struct TabelaZmiennych tabelaZmiennych, char* nazwa) {
	int hash = funkcjaHash(nazwa);
	ElementTablicy* elTab = tabelaZmiennych.elemTab[hash];
	while (elTab != NULL && !myStrEq(elTab->zmienna->nazwa, nazwa))
		elTab = elTab->nastepny;
	return (elTab == NULL ? NULL : elTab->zmienna);
}

void printTablica(struct TabelaZmiennych tabelaZmiennych) {
	struct ElementTablicy* pom;
	for (int i = 0; i < ROZMIAR_TABLICY_HASHOWEJ; i++) {
		if (tabelaZmiennych.elemTab[i] != NULL) {
			pom = tabelaZmiennych.elemTab[i];
			while (pom != NULL) {
				wypiszZmienna(*(pom->zmienna));
				pom = pom->nastepny;
			}
		}
	}
}
