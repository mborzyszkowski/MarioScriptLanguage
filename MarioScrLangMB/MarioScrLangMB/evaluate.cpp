#include "evaluate.h"
#include "tokeny.h"

void twoArgNonEmptyArgsOp(int oprationCode, int val1, int val2, struct Token* pomToken) {
	int result = 1;
	switch (oprationCode) {
	case OP_NEQ:
		if (val1 != val2)
			pomToken->intValue = 0;
		else
			pomToken->isEmpty = true;
		break;
	case OP_EQ:
		if (val1 == val2)
			pomToken->intValue = 0;
		else
			pomToken->isEmpty = true;
		break;
	case OP_LT:
		if (val1 < val2)
			pomToken->intValue = 0;
		else
			pomToken->isEmpty = true;
		break;
	case OP_GT:
		if (val1 > val2)
			pomToken->intValue = 0;
		else
			pomToken->isEmpty = true;
		break;
	case OP_LE:
		if (val1 <= val2)
			pomToken->intValue = 0;
		else
			pomToken->isEmpty = true;
		break;
	case OP_GE:
		if (val1 >= val2)
			pomToken->intValue = 0;
		else
			pomToken->isEmpty = true;
		break;
	case OP_PLUS:
		pomToken->intValue = val1 + val2;
		break;
	case OP_MINUS:
		pomToken->intValue = val1 - val2;
		break;
	case OP_DIV:
		if (val2 != 0)
			pomToken->intValue = val1 / val2;
		else
			pomToken->isEmpty = true;
		break;
	case OP_MULT:
		pomToken->intValue = val1 * val2;
		break;
	case OP_MOD:
		if (val2 != 0)
			pomToken->intValue = val1 % val2;
		else
			pomToken->isEmpty = true;
		break;
	}
}

bool isTokenEmpty(struct Token* token) {
	if ((token->typ == TOKEN_TYP_ZMIENNA && token->zmienna->jestPusta) ||
		(token->typ == TOKEN_TYP_LICZBA  && token->isEmpty))
		return true;
	return false;
}

int getTokenValue(struct Token* token) {
	if (token->typ == TOKEN_TYP_ZMIENNA)
		return token->zmienna->wartosc;
	else
		return token->intValue;
}

struct Token* twoArgOpLewy(struct Token* op, struct Token* arg1, struct Token* arg2, struct Token* pomToken) {
	if (isTokenEmpty(arg1) || isTokenEmpty(arg2))
		pomToken->isEmpty = true;
	else
		twoArgNonEmptyArgsOp(op->operacja, getTokenValue(arg1), getTokenValue(arg2), pomToken);
	return pomToken;
}

bool sprawdzCzyMozliweOperacjePrzyZwiekszeniu(int limitOperacji, int *liczbaOperacji) {
	if ((*liczbaOperacji) >= limitOperacji) {
		return false;
	}
	else {
		(*liczbaOperacji)++;
		return true;
	}
}

struct Token* twoArgOp(struct Token* op, struct Token* arg1, struct Token* arg2) {
	struct Token* pomToken = NULL;
	pomToken = (struct Token*)malloc(sizeof(struct Token));
	pomToken->typ = TOKEN_TYP_LICZBA;
	pomToken->arnosc = ARNOSC_ZERO;
	pomToken->intValue = 0;
	pomToken->zmienna = NULL;
	pomToken->operacja = OP_NOOP;
	pomToken->isEmpty = false;
	switch (op->operacja) {
	case OP_ASSIGN:
		if (arg1->typ == TOKEN_TYP_ZMIENNA) {
			if (arg2->typ == TOKEN_TYP_ZMIENNA)
				if (arg2->zmienna->jestPusta)
					arg1->zmienna->jestPusta = true;
				else {
					arg1->zmienna->jestPusta = false;
					arg1->zmienna->wartosc = arg2->zmienna->wartosc;
				}
			else if (arg2->typ == TOKEN_TYP_LICZBA) {
				if (arg2->isEmpty) arg1->zmienna->jestPusta = true;
				else {
					arg1->zmienna->jestPusta = false;
					arg1->zmienna->wartosc = arg2->intValue;
				}
			}
			pomToken->typ = TOKEN_TYP_ZMIENNA;
			pomToken->zmienna = arg1->zmienna;
		}
		break;
	case OP_OR:
		if (!isTokenEmpty(arg1) || !isTokenEmpty(arg2))
			pomToken->intValue = 0;
		else
			pomToken->isEmpty = true;
		break;
	case OP_AND:
		if (!isTokenEmpty(arg1) && !isTokenEmpty(arg2))
			pomToken->intValue = 0;
		else
			pomToken->isEmpty = true;
		break;
	case OP_LT:
	case OP_GT:
	case OP_LE:
	case OP_GE:
	case OP_NEQ:
	case OP_EQ:
	case OP_PLUS:
	case OP_MINUS:
	case OP_DIV:
	case OP_MULT:
	case OP_MOD:
		if (isTokenEmpty(arg1) || isTokenEmpty(arg2))
			pomToken->isEmpty = true;
		else
			twoArgNonEmptyArgsOp(op->operacja, getTokenValue(arg1), getTokenValue(arg2), pomToken);
		break;
	} //end switch
	return pomToken;
}

struct Token*  oneArgOp(struct Token* op, struct Token* arg) {
	struct Token* pomToken = NULL;
	pomToken = (struct Token*)malloc(sizeof(struct Token));
	pomToken->typ = TOKEN_TYP_LICZBA;
	pomToken->arnosc = ARNOSC_ZERO;
	pomToken->intValue = 0;
	pomToken->zmienna = NULL;
	pomToken->operacja = OP_NOOP;
	pomToken->isEmpty = false;
	switch (op->operacja) {
	case OP_MINUS:
		if (!isTokenEmpty(arg))
			pomToken->intValue = -getTokenValue(arg);
		else
			pomToken->isEmpty = true;
		break;
	case OP_NEG:
		if (isTokenEmpty(arg))
			pomToken->intValue = 0;
		else
			pomToken->isEmpty = true;
		break;
	}
	return pomToken;
}