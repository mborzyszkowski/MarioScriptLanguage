#ifndef TABLICATOKENOW_H
#define TABLICATOKENOW_H

#include "tokeny.h"

struct TablicaTokenow {
	struct Token *token;
	struct TablicaTokenow *nastepny;
	struct TablicaTokenow *poprzedni;
};

struct TablicaTokenow*  newStosTokenow(struct Token* token);
void pushTabToken(struct TablicaTokenow** stosTokenow, struct TablicaTokenow* tabToken);
void pushToken(struct TablicaTokenow** stosTokenow, struct Token* token);
struct Token* popToken(struct TablicaTokenow** stosTokenow);
void addHeadTabTokenList(struct TablicaTokenow** firstOnList, struct TablicaTokenow* element);
struct TablicaTokenow* getNextTabTokenFromList(struct TablicaTokenow** firstOnList);
struct Token* getNextTokenFromList(struct TablicaTokenow** firstOnList);
struct TablicaTokenow* previewNextTabTokenFromList(struct TablicaTokenow** firstOnList);
struct Token* previewNextTokenFromList(struct TablicaTokenow** firstOnList);
struct TablicaTokenow* refOnFirstTabToken(struct TablicaTokenow* lastOnList);
struct TablicaTokenow* kopiaTabTokenow(struct TablicaTokenow* tablica);
int tabTokenSize(struct TablicaTokenow* tablica);
void wypiszTabTokens(struct TablicaTokenow* tablica);
void freeTablicaTokenow(struct TablicaTokenow* tablica);

#endif // TABLICATOKENOW_H