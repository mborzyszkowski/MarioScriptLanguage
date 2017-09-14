#include "tablicaTokenow.h"

struct TablicaTokenow*  newStosTokenow(struct Token* token) {
	struct TablicaTokenow* wynik = (struct TablicaTokenow*)malloc(sizeof(struct TablicaTokenow));
	wynik->token = token;
	wynik->nastepny = NULL;
	wynik->poprzedni = NULL;
	return wynik;
}

void pushTabToken(struct TablicaTokenow** stosTokenow, struct TablicaTokenow* tabToken) {
	if ((*stosTokenow) == NULL) {
		(*stosTokenow) = tabToken;
	}
	else {
		tabToken->nastepny = *stosTokenow;
		tabToken->poprzedni = NULL;
		(*stosTokenow)->poprzedni = tabToken;
		(*stosTokenow) = tabToken;
	}
}

void pushToken(struct TablicaTokenow** stosTokenow, struct Token* token) {
	pushTabToken(stosTokenow, newStosTokenow(token));
}

struct Token* popToken(struct TablicaTokenow** stosTokenow) {
	struct TablicaTokenow* pom = *stosTokenow;
	struct Token* wynik = NULL;
	if ((*stosTokenow) == NULL)
		return NULL;
	else {
		wynik = (*stosTokenow)->token;
		(*stosTokenow) = (*stosTokenow)->nastepny;
		if ((*stosTokenow) != NULL)
			(*stosTokenow)->poprzedni = NULL;
		free(pom);
		return wynik;
	}
}

void addHeadTabTokenList(struct TablicaTokenow** firstOnList, struct TablicaTokenow* element) {
	struct TablicaTokenow* pom = element;
	element->poprzedni = (*firstOnList);
	(*firstOnList)->nastepny = element;
	(*firstOnList) = element;
}

struct TablicaTokenow* getNextTabTokenFromList(struct TablicaTokenow** firstOnList) {
	struct TablicaTokenow* pom = *firstOnList;
	struct TablicaTokenow* wynik = NULL;

	if ((*firstOnList) == NULL)
		return NULL;
	else {
		wynik = (*firstOnList);
		(*firstOnList) = (*firstOnList)->poprzedni;
		wynik->poprzedni = NULL;
		if ((*firstOnList) != NULL)
			(*firstOnList)->nastepny = NULL;
		return wynik;
	}
}

// bierze token z dna stosu i usuwa go (traktuje jak listê)
// firstOnList = dno stosu
struct Token* getNextTokenFromList(struct TablicaTokenow** firstOnList) {
	struct TablicaTokenow* pom = getNextTabTokenFromList(firstOnList);
	struct Token* wynik = NULL;

	if (pom == NULL)
		return NULL;
	else {
		wynik = pom->token;
		free(pom);
		return wynik;
	}
}


// bierze token z dna stosu i nie usuwa go (traktuje jak listê)
// firstOnList = dno stosu
struct TablicaTokenow* previewNextTabTokenFromList(struct TablicaTokenow** firstOnList) {
	if ((*firstOnList) == NULL)
		return NULL;
	else {
		return (*firstOnList);
	}
}

// bierze token z dna stosu i nie usuwa go (traktuje jak listê)
// firstOnList = dno stosu
struct Token* previewNextTokenFromList(struct TablicaTokenow** firstOnList) {
	if ((*firstOnList) == NULL)
		return NULL;
	else {
		return (*firstOnList)->token;
	}
}

struct TablicaTokenow* refOnFirstTabToken(struct TablicaTokenow* lastOnList) {
	struct TablicaTokenow* pom = lastOnList;
	if (pom != NULL)
		while (pom->nastepny != NULL)
			pom = pom->nastepny;
	return pom;
}

struct TablicaTokenow* kopiaTabTokenow(struct TablicaTokenow* tablica) {
	struct TablicaTokenow* wynik = NULL, *pomWyn1 = NULL, *pomWyn2 = NULL;
	if (tablica == NULL)
		return NULL;
	else {
		pomWyn1 = newStosTokenow(kopiaTokenu(tablica->token));
		wynik = pomWyn1;
		while (tablica->poprzedni != NULL) {
			tablica = tablica->poprzedni;
			pomWyn2 = pomWyn1;
			pomWyn1 = newStosTokenow(kopiaTokenu(tablica->token));
			pomWyn1->nastepny = pomWyn2;
			pomWyn2->poprzedni = pomWyn1;
		}
		return wynik;
	}
}

int tabTokenSize(struct TablicaTokenow* tablica) {
	struct TablicaTokenow* pom = tablica;
	int cnt = 0;
	while (pom != NULL) {
		pom = pom->poprzedni;
		cnt++;
	}
	return cnt;
}

void wypiszTabTokens(struct TablicaTokenow* tablica) {
	struct TablicaTokenow* pom = tablica;
	while (pom != NULL) {
		if (pom->token->typ == TOKEN_TYP_ZMIENNA)
			printf("%s ", pom->token->zmienna->nazwa);
		else if (pom->token->typ == TOKEN_TYP_LICZBA) 
			printf("%d ", pom->token->intValue);
		else if (pom->token->typ == TOKEN_TYP_OPERACJA)
			printf("%d ", pom->token->operacja);
		pom = pom->poprzedni;
	}

}

void freeTablicaTokenow(struct TablicaTokenow* tablica) {
	struct TablicaTokenow* pom = tablica;
	while (pom != NULL) {
		pom = tablica->poprzedni;
		free(tablica);
		tablica = pom;
	}
	//if (tablica != NULL) {
	//	freeTablicaTokenow(tablica->poprzedni);
	//}
	//else {
	//	free(tablica);
	//}
}