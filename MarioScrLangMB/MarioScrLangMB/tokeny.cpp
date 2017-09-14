#include "tokeny.h"

void dodajZmiennaDoListy(struct ListaZmiennych ** listaZmiennych, struct Zmienna *zmienna) {
	struct ListaZmiennych *pom = (struct ListaZmiennych*)malloc(sizeof(struct ListaZmiennych));
	struct ListaZmiennych *pom2 = NULL;
	pom->zmienna = zmienna;
	pom->nastepna = NULL;
	if ((*listaZmiennych) == NULL)
		(*listaZmiennych) = pom;
	else {
		pom2 = (*listaZmiennych);
		while (pom2->nastepna != NULL)
			pom2 = pom2->nastepna;
		pom2->nastepna = pom;
	}
}

void wypiszListeZmiennych(struct ListaZmiennych * listaZmiennych) {
	struct ListaZmiennych *pom = listaZmiennych;
	while (pom != NULL) {
		printf("%s ", pom->zmienna->nazwa);
		pom = pom->nastepna;
	}
}

void czytajZmienne(struct TabelaZmiennych* tabelaZmiennych, char* wiersz, struct ListaZmiennych ** listaZmiennych) {
	char buff[1024];
	struct Zmienna* zmienna;

	char *pom = wiersz, *pom2;
	int cnt = 0;
	while (*pom != '\0' && *pom != '\n') {
		if (jestLitera(*pom))
			buff[cnt++] = *pom;
		else {
			if (jestBialy(*pom)) {
				buff[cnt++] = '\0';
				zmienna = (struct Zmienna*)malloc(sizeof(struct Zmienna));
				pom2 = (char*)malloc((cnt) * sizeof(char));
				strCopy(pom2, buff);
				cnt = 0;
				zmienna->nazwa = pom2;
				zmienna->jestPusta = true;
				zmienna->wartosc = 0;
				dodajDoTablicy(tabelaZmiennych, zmienna);
				dodajZmiennaDoListy(listaZmiennych, zmienna);
			}
			else cnt = 0;
		}
		pom++;
	}
	if (cnt > 0) {
		buff[cnt++] = '\0';
		zmienna = (struct Zmienna*)malloc(sizeof(struct Zmienna));
		pom2 = (char*)malloc((cnt) * sizeof(char));
		strCopy(pom2, buff);
		cnt = 0;
		zmienna->nazwa = pom2;
		zmienna->jestPusta = true;
		zmienna->wartosc = 0;
		dodajDoTablicy(tabelaZmiennych, zmienna);
		dodajZmiennaDoListy(listaZmiennych, zmienna);
	}
}

int czytajLiczbe(char* wiersz, int *startPos) {
	int pomPos = *startPos;
	while (jestCyfra(wiersz[pomPos])) pomPos++;
	char pomStr[MAKSYMALNY_ROZMIAR_LICZBY];
	for (int i = *startPos; i < pomPos; i++)
		pomStr[i - *startPos] = wiersz[i];
	pomStr[pomPos - *startPos] = '\0';
	*startPos = pomPos;
	return atoi(pomStr);
}

struct Zmienna* czytajZmienna(struct TabelaZmiennych* tabelaZmiennych,
	char* wiersz, int *startPos)
{
	struct Zmienna* pomZm = NULL;
	int pomPos = *startPos;
	while (jestLitera(wiersz[pomPos])) pomPos++;
	char pomStr[MAKSYMALNY_ROZMIAR_ZMIENNEJ];
	for (int i = *startPos; i < pomPos; i++)
		pomStr[i - *startPos] = wiersz[i];
	pomStr[pomPos - *startPos] = '\0';

	pomZm = znajdzWTablicy(*tabelaZmiennych, pomStr);
	if (pomZm == NULL) {
		pomZm = (struct Zmienna*)malloc(sizeof(struct Zmienna));
		char * pom2 = (char*)malloc((pomPos - *startPos + 1) * sizeof(char));
		strCopy(pom2, pomStr);
		pomZm->nazwa = pom2;
		pomZm->jestPusta = true;
		pomZm->wartosc = 0;
		dodajDoTablicy(tabelaZmiennych, pomZm);
	}
	*startPos = pomPos;
	return pomZm;
}
//---------------------------------------------
// 0 = brak tokena w linii
// 1 = odda³em token
// -1 = koniec wiersza, brak tokenu

int nastepnyTokienZWiersza(struct TabelaZmiennych* tabelaZmiennych,
	char* wiersz, int *startPos, struct Token** tokenResult) {
	int liczba, result = 0;
	struct Zmienna* zmienna;
	// pomin bia³e znaki
	while (!koniecWiersza(wiersz[*startPos]) && jestBialy(wiersz[*startPos]))
		(*startPos)++;
	if (koniecWiersza(wiersz[*startPos])) {
		result = -1;
	}
	else
		if (jestCyfra(wiersz[*startPos])) {  // liczba
			liczba = czytajLiczbe(wiersz, startPos);
			*tokenResult = (struct Token*)malloc(sizeof(struct Token));
			(*tokenResult)->typ = TOKEN_TYP_LICZBA;
			(*tokenResult)->arnosc = ARNOSC_ZERO;
			(*tokenResult)->intValue = liczba;
			(*tokenResult)->zmienna = NULL;
			(*tokenResult)->operacja = OP_NOOP;
			(*tokenResult)->isEmpty = false;
			result = 1;
		}
		else
			if (jestLitera(wiersz[*startPos])) { // zmienna
				zmienna = czytajZmienna(tabelaZmiennych, wiersz, startPos);
				if (zmienna == NULL) {
					result = 0;
				}
				else {
					*tokenResult = (struct Token*)malloc(sizeof(struct Token));
					(*tokenResult)->typ = TOKEN_TYP_ZMIENNA;
					(*tokenResult)->arnosc = ARNOSC_ZERO;
					(*tokenResult)->intValue = 0;
					(*tokenResult)->zmienna = zmienna;
					(*tokenResult)->operacja = OP_NOOP;
					(*tokenResult)->isEmpty = true;
					result = 1;
				}
			}
			else {
				*tokenResult = (struct Token*)malloc(sizeof(struct Token));
				(*tokenResult)->typ = TOKEN_TYP_OPERACJA;
				(*tokenResult)->intValue = 0;
				(*tokenResult)->zmienna = NULL;
				(*tokenResult)->isEmpty = false;
				result = 1;
				switch (wiersz[*startPos]) {
				case '=':
					if (wiersz[(*startPos) + 1] == '=') {
						(*tokenResult)->operacja = OP_EQ;
						(*tokenResult)->arnosc = ARNOSC_DWA;
						(*tokenResult)->priorytet = PRIORYTET_4;
						*startPos += 2;
					}
					else {
						(*tokenResult)->operacja = OP_ASSIGN;
						(*tokenResult)->arnosc = ARNOSC_DWA;
						(*tokenResult)->priorytet = PRIORYTET_1;
						*startPos += 1;
					}
					break;
				case '|':
					(*tokenResult)->operacja = OP_OR;
					(*tokenResult)->arnosc = ARNOSC_DWA;
					(*tokenResult)->priorytet = PRIORYTET_2;
					*startPos += 1;
					break;
				case '&':
					(*tokenResult)->operacja = OP_AND;
					(*tokenResult)->arnosc = ARNOSC_DWA;
					(*tokenResult)->priorytet = PRIORYTET_3;
					*startPos += 1;
					break;
				case '!':
					if (wiersz[(*startPos) + 1] == '=') {
						(*tokenResult)->operacja = OP_NEQ;
						(*tokenResult)->arnosc = ARNOSC_DWA;
						(*tokenResult)->priorytet = PRIORYTET_4;
						*startPos += 2;
					}
					else {
						(*tokenResult)->operacja = OP_NEG;
						(*tokenResult)->arnosc = ARNOSC_JEDEN;
						(*tokenResult)->priorytet = PRIORYTET_8;
						*startPos += 1;
					}
					break;
				case '<':
					if (wiersz[(*startPos) + 1] == '=') {
						(*tokenResult)->operacja = OP_LE;
						(*tokenResult)->arnosc = ARNOSC_DWA;
						(*tokenResult)->priorytet = PRIORYTET_5;
						*startPos += 2;
					}
					else {
						(*tokenResult)->operacja = OP_LT;
						(*tokenResult)->arnosc = ARNOSC_DWA;
						(*tokenResult)->priorytet = PRIORYTET_5;
						*startPos += 1;
					}
					break;
				case '>':
					if (wiersz[(*startPos) + 1] == '=') {
						(*tokenResult)->operacja = OP_GE;
						(*tokenResult)->arnosc = ARNOSC_DWA;
						(*tokenResult)->priorytet = PRIORYTET_5;
						*startPos += 2;
					}
					else {
						(*tokenResult)->operacja = OP_GT;
						(*tokenResult)->arnosc = ARNOSC_DWA;
						(*tokenResult)->priorytet = PRIORYTET_5;
						*startPos += 1;
					}
					break;
				case '+':
					(*tokenResult)->operacja = OP_PLUS;
					(*tokenResult)->arnosc = ARNOSC_DWA;
					(*tokenResult)->priorytet = PRIORYTET_6;
					*startPos += 1;
					break;
				case '-':
					(*tokenResult)->operacja = OP_MINUS;
					(*tokenResult)->arnosc = ARNOSC_DWA;
					(*tokenResult)->priorytet = PRIORYTET_6;
					*startPos += 1;
					break;
				case '/':
					(*tokenResult)->operacja = OP_DIV;
					(*tokenResult)->arnosc = ARNOSC_DWA;
					(*tokenResult)->priorytet = PRIORYTET_7;
					*startPos += 1;
					break;
				case '*':
					(*tokenResult)->operacja = OP_MULT;
					(*tokenResult)->arnosc = ARNOSC_DWA;
					(*tokenResult)->priorytet = PRIORYTET_7;
					*startPos += 1;
					break;
				case '%':
					(*tokenResult)->operacja = OP_MOD;
					(*tokenResult)->arnosc = ARNOSC_DWA;
					(*tokenResult)->priorytet = PRIORYTET_7;
					*startPos += 1;
					break;
				case '(':
					(*tokenResult)->operacja = OP_LPAR1;
					(*tokenResult)->arnosc = ARNOSC_ZERO;
					*startPos += 1;
					break;
				case ')':
					(*tokenResult)->operacja = OP_RPAR1;
					(*tokenResult)->arnosc = ARNOSC_ZERO;
					*startPos += 1;
					break;
				case '{':
					(*tokenResult)->operacja = OP_LPAR2;
					(*tokenResult)->arnosc = ARNOSC_ZERO;
					*startPos += 1;
					break;
				case '}':
					(*tokenResult)->operacja = OP_RPAR2;
					(*tokenResult)->arnosc = ARNOSC_ZERO;
					*startPos += 1;
					break;
				case '?':
					(*tokenResult)->operacja = OP_WARUNEK;
					(*tokenResult)->arnosc = ARNOSC_ZERO;
					*startPos += 1;
					break;
				case '@':
					(*tokenResult)->operacja = OP_WHILE;
					(*tokenResult)->arnosc = ARNOSC_ZERO;
					*startPos += 1;
					break;
				default:
					result = 0;
					break;
				}
			}
			return result;
}

//---------------------------------------------
// 0 = beak tokena / b³êdny token
// 1 = odda³em token
// -1 = koniec pliku

int nastepnyTokien(struct TabelaZmiennych* tabelaZmiennych, char* wiersz, int *startPos,
	struct Token** tokenResult) {
	int wynik;
	while ((wynik = nastepnyTokienZWiersza(tabelaZmiennych, wiersz, startPos, tokenResult)) == -1) {
		if (fgets(wiersz, LINE_SIZE, stdin) == NULL)
			return -1;
		else {
			*startPos = 0;
		}
	}
	return wynik;
}

struct Token* kopiaTokenu(struct Token* token) {
	struct Token *kopia = (struct Token*)malloc(sizeof(struct Token));
	kopia->typ = token->typ;
	kopia->arnosc = token->arnosc;
	kopia->intValue = token->intValue;
	kopia->isEmpty = token->isEmpty;
	kopia->operacja = token->operacja;
	kopia->priorytet = token->priorytet;
	kopia->zmienna = token->zmienna;
	return kopia;
}

void freeToken(struct Token* token) {
	if (token != NULL) {
		free(token);
		token = NULL;
	}
}

