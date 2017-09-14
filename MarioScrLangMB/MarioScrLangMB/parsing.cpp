#include "parsing.h"

int nastepnyTokienNieZero(struct TabelaZmiennych* tabelaZmiennych,
	char* wiersz, int *wierszPos, struct Token** token) {
	int wynik;
	while ((wynik = nastepnyTokien(tabelaZmiennych, wiersz, wierszPos, token)) == 0) {}
	return wynik;
}

void intTablicaTokenow(struct TabelaZmiennych* tabelaZmiennych,
	struct TablicaTokenow** firstReadToken,
	struct TablicaTokenow** lastReadToken) {
	int wierszPos = 0;
	struct Token* token;

	char wiersz[LINE_SIZE];
	(*firstReadToken) = NULL;
	(*lastReadToken) = NULL;

	fgets(wiersz, LINE_SIZE, stdin);
	while (nastepnyTokienNieZero(tabelaZmiennych, wiersz, &wierszPos, &token) == 1) {
		pushToken(lastReadToken, token);
		if ((*firstReadToken) == NULL) {
			(*firstReadToken) = (*lastReadToken);
		}
	}
}

void stacksEvaluation(struct TablicaTokenow** outputStack,
	struct TablicaTokenow** operationStack,
	bool doLewegoNawiasu,
	int limitOperacji, int *liczbaOperacji)
{
	struct Token* op2Token = NULL;
	struct Token* arg1Token = NULL;
	struct Token* arg2Token = NULL;

	while ((*operationStack) != NULL) {
		op2Token = popToken(operationStack);
		if (doLewegoNawiasu && op2Token->operacja == OP_LPAR1)
			break;
		if (op2Token->arnosc == ARNOSC_DWA) {
			if (arg2Token = popToken(outputStack)) {
				if (arg1Token = popToken(outputStack)) {
					if (!sprawdzCzyMozliweOperacjePrzyZwiekszeniu(limitOperacji, liczbaOperacji)) {
						return;
					}
					pushToken(outputStack, twoArgOp(op2Token, arg1Token, arg2Token));
					free(op2Token);
					free(arg1Token);
					free(arg2Token);
				}
				else return;
			}
			else return;
		}
		else if (op2Token->arnosc == ARNOSC_JEDEN) {
			if (arg1Token = popToken(outputStack)) {
				if (!sprawdzCzyMozliweOperacjePrzyZwiekszeniu(limitOperacji, liczbaOperacji)) {
					return;
				}
				pushToken(outputStack, oneArgOp(op2Token, arg1Token));
				free(op2Token);
				free(arg1Token);
			}
			else return;
		}
	}
}

struct TablicaTokenow* czytajWyrazenie(struct TablicaTokenow** tokenTab, int *wenPar, bool inCond) {
	struct TablicaTokenow* wynik = NULL, *curr = NULL, *next = NULL, *pom = NULL;
	while ((*tokenTab) != NULL) {
		curr = getNextTabTokenFromList(tokenTab);
		next = previewNextTabTokenFromList(tokenTab);
		if (inCond && curr->token->typ == TOKEN_TYP_OPERACJA)
			if (curr->token->operacja == OP_LPAR1)
				*wenPar += 1;
			else if (curr->token->operacja == OP_RPAR1)
				*wenPar -= 1;
		if (next == NULL || (next->token->typ == TOKEN_TYP_OPERACJA && next->token->operacja == OP_RPAR2)) {
			pushTabToken(&wynik, curr);
			break;
		}
		else if ((next->token->typ == TOKEN_TYP_LICZBA || next->token->typ == TOKEN_TYP_ZMIENNA ||
			(next->token->typ == TOKEN_TYP_OPERACJA &&
			(next->token->operacja == OP_WARUNEK ||
				next->token->operacja == OP_WHILE ||
				next->token->operacja == OP_LPAR1))
			) &&
			(curr->token->typ == TOKEN_TYP_LICZBA || curr->token->typ == TOKEN_TYP_ZMIENNA ||
			(curr->token->typ == TOKEN_TYP_OPERACJA && curr->token->operacja == OP_RPAR1))) {
			pushTabToken(&wynik, curr);
			break;

		}
		else {
			pushTabToken(&wynik, curr);
			if (inCond && next->token->typ == TOKEN_TYP_OPERACJA
				&& next->token->operacja == OP_RPAR1 && (*wenPar == 1))
			{
				break;
			}
		}
	}
	return refOnFirstTabToken(wynik);
}

struct ProgramTree* czytajKomende(struct TablicaTokenow** tablicaTokenow) {
	struct TablicaTokenow *tabToken = NULL, *pom = NULL;
	tabToken = getNextTabTokenFromList(tablicaTokenow);
	struct ProgramTree* wynik = (struct ProgramTree*)malloc(sizeof(struct ProgramTree));
	struct ProgramTree* pomProg = NULL, *pomProg2 = NULL;
	int parCnt = 0;

	if (tabToken != NULL)
		if (tabToken->token->operacja == OP_WARUNEK) {
			pom = getNextTabTokenFromList(tablicaTokenow); // nawias ( po ?
			if (pom != NULL && pom->token->typ == TOKEN_TYP_OPERACJA && pom->token->operacja == OP_LPAR1) {
				wynik->typWezla = PROGR_IF;
				wynik->nastepepnaKomenda = NULL;
				parCnt = 1;
				wynik->warunek = czytajWyrazenie(tablicaTokenow, &parCnt, true);
				pom = getNextTabTokenFromList(tablicaTokenow); // nawias ) po ?
				if (pom != NULL && pom->token->typ == TOKEN_TYP_OPERACJA && pom->token->operacja == OP_RPAR1) {
					pom = getNextTabTokenFromList(tablicaTokenow); // nawias { po ?
					if (pom != NULL && pom->token->typ == TOKEN_TYP_OPERACJA && pom->token->operacja == OP_LPAR2) {
						wynik->komenda = czytajListeKomend(tablicaTokenow, true);
						pom = getNextTabTokenFromList(tablicaTokenow); // nawias } po ?
						if (pom != NULL && pom->token->typ == TOKEN_TYP_OPERACJA && pom->token->operacja == OP_RPAR2) {
						}
						else wynik = NULL;
					}
				}
			}
		}
		else if (tabToken->token->operacja == OP_WHILE) {
			pom = getNextTabTokenFromList(tablicaTokenow); // nawias ( po @
			if (pom != NULL && pom->token->typ == TOKEN_TYP_OPERACJA && pom->token->operacja == OP_LPAR1) {
				wynik->typWezla = PROGR_WHILE;
				wynik->nastepepnaKomenda = NULL;
				parCnt = 1;
				wynik->warunek = czytajWyrazenie(tablicaTokenow, &parCnt, true);
				pom = getNextTabTokenFromList(tablicaTokenow); // nawias ) po @
				if (pom != NULL && pom->token->typ == TOKEN_TYP_OPERACJA && pom->token->operacja == OP_RPAR1) {
					pom = getNextTabTokenFromList(tablicaTokenow); // nawias { po @
					if (pom != NULL && pom->token->typ == TOKEN_TYP_OPERACJA && pom->token->operacja == OP_LPAR2) {
						wynik->komenda = czytajListeKomend(tablicaTokenow, true);
						pom = getNextTabTokenFromList(tablicaTokenow); // nawias } po @
						if (pom != NULL && pom->token->typ == TOKEN_TYP_OPERACJA && pom->token->operacja == OP_RPAR2) {
						}
						else wynik = NULL;
					}
				}
			}
		}
		else { // wyra¿enie
			wynik->typWezla = PROGR_WYR;
			addHeadTabTokenList(tablicaTokenow, tabToken);
			wynik->warunek = czytajWyrazenie(tablicaTokenow, 0, false);
			wynik->komenda = NULL;
			wynik->nastepepnaKomenda = NULL;
			//printf("Liczba tokenów: %d\n", tabTokenSize(wynik->warunek) );
		}
	else return NULL;

	return wynik;
}

void freeKomenda(struct ProgramTree* prog) {
	if (prog != NULL) {
		if (prog->nastepepnaKomenda != NULL)
			freeKomenda(prog->nastepepnaKomenda);
		if(prog->komenda != NULL)
			freeKomenda(prog->komenda);
		if (prog->warunek != NULL)
			freeTablicaTokenow(prog->warunek);
	}
}
struct ProgramTree* czytajListeKomend(struct TablicaTokenow** tokenTab, bool inCommand) {
	struct ProgramTree* wynik = NULL, *pom = NULL, *pom2 = NULL, *curr = NULL;
	struct TablicaTokenow* next = NULL;

	if (inCommand) {
		next = previewNextTabTokenFromList(tokenTab);
		if (next != NULL && next->token->typ == TOKEN_TYP_OPERACJA && next->token->operacja == OP_RPAR2) {
			return NULL;
		}
	}

	wynik = (struct ProgramTree*)malloc(sizeof(struct ProgramTree));
	wynik->komenda = NULL;
	wynik->warunek = NULL;
	wynik->nastepepnaKomenda = NULL;
	wynik->typWezla = -1;
	curr = wynik;
	bool czyCzytac = true;
	while (czyCzytac && (pom = czytajKomende(tokenTab)) != NULL) {
		if (curr == NULL) {
			curr = (struct ProgramTree*)malloc(sizeof(struct ProgramTree));
			pom2->nastepepnaKomenda = curr;
		}
		curr->typWezla = PROGR_COMP;
		curr->komenda = pom;
		curr->warunek = NULL;
		curr->nastepepnaKomenda = NULL;
		pom2 = curr;
		curr = curr->nastepepnaKomenda;
		if (inCommand) {
			next = previewNextTabTokenFromList(tokenTab);
			if (next != NULL && next->token->typ == TOKEN_TYP_OPERACJA && next->token->operacja == OP_RPAR2) {
				czyCzytac = false;
			}
		}
	}
	if (wynik->typWezla == -1) {
		free(wynik);
		wynik = NULL;
	}
	return wynik;
}



//
//  WYLICZ WYRAZNIE
//
struct Token* wyliczWyrazenie(struct TablicaTokenow* wyr,
	struct TablicaTokenow** outputStack,
	struct TablicaTokenow** operationStack,
	int limitOperacji, int *liczbaOperacji)
{
	// kopia wyra¿enia
	struct TablicaTokenow* wyrKopia = kopiaTabTokenow(wyr);

	struct Token *token = NULL, *lastToken = NULL, *nextToken = NULL;

	struct Token* op2Token = NULL;
	struct Token* arg1Token = NULL;
	struct Token* arg2Token = NULL;
	bool doWrzucenia = 0;//

	while ((token = getNextTokenFromList(&wyrKopia)) != NULL) {
		// minus
		if (token->typ == TOKEN_TYP_OPERACJA && token->operacja == OP_MINUS)
			if ((lastToken != NULL &&  lastToken->typ == TOKEN_TYP_OPERACJA &&
				lastToken->operacja != OP_RPAR1) || lastToken == NULL) {
				nextToken = previewNextTokenFromList(&wyrKopia);
				if (nextToken != NULL && nextToken->typ == TOKEN_TYP_LICZBA) { // -liczba nie jest operacj¹
					freeToken(token);
					token = getNextTokenFromList(&wyrKopia);
					token->intValue = -token->intValue;
				}
				else {
					token->arnosc = ARNOSC_JEDEN;
					token->priorytet = PRIORYTET_8;
				}
			}

		if (token->typ == TOKEN_TYP_LICZBA || token->typ == TOKEN_TYP_ZMIENNA) {
			pushToken(outputStack, token);
		}
		else
			if (token->typ == TOKEN_TYP_OPERACJA) {
				if ((*operationStack) == NULL || token->operacja == OP_LPAR1) {
					pushToken(operationStack, token);
				}
				else if (token->operacja == OP_RPAR1) {
					stacksEvaluation(outputStack, operationStack, true, limitOperacji, liczbaOperacji);
				}
				else {
					while (op2Token = popToken(operationStack)) {
						if (token->operacja == OP_ASSIGN) { // jedyna prawostronna
							if (token->priorytet < op2Token->priorytet) {
								if (arg2Token = popToken(outputStack)) {
									if (arg1Token = popToken(outputStack)) {
										if (!sprawdzCzyMozliweOperacjePrzyZwiekszeniu(limitOperacji, liczbaOperacji)) {
											return NULL;
										}
										pushToken(outputStack, twoArgOp(op2Token, arg1Token, arg2Token));
										freeToken(op2Token);
										freeToken(arg1Token);
										freeToken(arg2Token);
										doWrzucenia = 1;
									}
									else return NULL;
								}
								else return NULL;
							}
							else {
								pushToken(operationStack, op2Token);
								pushToken(operationStack, token);
								doWrzucenia = 0;
								break;
							}
						}
						else
							if (token->priorytet <= op2Token->priorytet) { // operacja lewostronna
								if (op2Token->arnosc == ARNOSC_DWA)
									if (arg2Token = popToken(outputStack)) {
										if (arg1Token = popToken(outputStack)) {
											if (!sprawdzCzyMozliweOperacjePrzyZwiekszeniu(limitOperacji, liczbaOperacji)) {
												return NULL;
											}
											pushToken(outputStack, twoArgOp(op2Token, arg1Token, arg2Token));
											freeToken(op2Token);
											freeToken(arg1Token);
											freeToken(arg2Token);
											doWrzucenia = 1;
										}
										else return NULL;
									}
									else return NULL;
								else if (op2Token->arnosc == ARNOSC_JEDEN) {
									if (arg1Token = popToken(outputStack)) {
										if (!sprawdzCzyMozliweOperacjePrzyZwiekszeniu(limitOperacji, liczbaOperacji)) {
											return NULL;
										}
										pushToken(outputStack, oneArgOp(op2Token, arg1Token));
										freeToken(op2Token);
										freeToken(arg1Token);
										doWrzucenia = 1;

									}
									else return NULL;
								}
							}
							else {
								pushToken(operationStack, op2Token);
								pushToken(operationStack, token);
								doWrzucenia = 0;
								break;
							}
					}
					if (doWrzucenia) {
						pushToken(operationStack, token);
					}
				}
			}
		lastToken = token;
	} // end while
	  // wykonaj operacje na stosie
	stacksEvaluation(outputStack, operationStack, false, limitOperacji, liczbaOperacji);
	if ((*outputStack) == NULL)
		return NULL;
	else
		return (*outputStack)->token;
}

//
//  WYKONAJ PROGRAM
//
void wykonajProgram(struct ProgramTree** program,
	struct TablicaTokenow** outputStack,
	struct TablicaTokenow** operationStack,
	int limitOperacji, int *liczbaOperacji)
{
	struct Token *warWynik = NULL;
	switch ((*program)->typWezla) {
	case PROGR_COMP:
		wykonajProgram(&((*program)->komenda), outputStack, operationStack, limitOperacji, liczbaOperacji);
		if ((*program)->nastepepnaKomenda)
			wykonajProgram(&((*program)->nastepepnaKomenda), outputStack, operationStack, limitOperacji, liczbaOperacji);
		break;
	case PROGR_IF:
		warWynik = wyliczWyrazenie((*program)->warunek, outputStack, operationStack, limitOperacji, liczbaOperacji);
		if (!sprawdzCzyMozliweOperacjePrzyZwiekszeniu(limitOperacji, liczbaOperacji)) {
			return;
		}
		// sprawdz czy nie falsz
		if ((warWynik->typ == TOKEN_TYP_LICZBA && !(warWynik->isEmpty)) ||
			(warWynik->typ == TOKEN_TYP_ZMIENNA && !(warWynik->zmienna->jestPusta)))
			if ((*program)->komenda)
				wykonajProgram(&((*program)->komenda), outputStack, operationStack, limitOperacji, liczbaOperacji);
		break;
	case PROGR_WHILE:
		warWynik = wyliczWyrazenie((*program)->warunek, outputStack, operationStack, limitOperacji, liczbaOperacji);
		if (!sprawdzCzyMozliweOperacjePrzyZwiekszeniu(limitOperacji, liczbaOperacji)) {
			return;
		}
		// sprawdz czy nie falsz
		if ((warWynik->typ == TOKEN_TYP_LICZBA && !(warWynik->isEmpty)) ||
			(warWynik->typ == TOKEN_TYP_ZMIENNA && !(warWynik->zmienna->jestPusta))) {
			if ((*program)->komenda)
				wykonajProgram(&((*program)->komenda), outputStack, operationStack, limitOperacji, liczbaOperacji);
			// wroc na poczatek
			wykonajProgram(program, outputStack, operationStack, limitOperacji, liczbaOperacji);
		}
		break;
	case PROGR_WYR:
		if ((*program)->warunek) {
			wyliczWyrazenie((*program)->warunek, outputStack, operationStack, limitOperacji, liczbaOperacji);
		}
		break;
	}
}


void parsuj(struct TabelaZmiennych* tabelaZmiennych, int limitOperacji, int *liczbaOperacji) {

	// pierwszy i ostani token 
	struct TablicaTokenow* firstReadToken;
	struct TablicaTokenow* lastReadToken;

	struct TablicaTokenow* outputStack = NULL;
	struct TablicaTokenow* operationStack = NULL;

	intTablicaTokenow(tabelaZmiennych, &firstReadToken, &lastReadToken);

	struct ProgramTree* program = NULL;
	while ((program = czytajKomende(&firstReadToken)) != NULL) {
		wykonajProgram(&program, &outputStack, &operationStack, limitOperacji, liczbaOperacji);
		freeKomenda(program);
	}
}
