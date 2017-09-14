#ifndef EVALUATE_H
#define EVALUATE_H

#include "tokeny.h"

void twoArgNonEmptyArgsOp(int oprationCode, int val1, int val2, struct Token* pomToken);
bool isTokenEmpty(struct Token* token);
int getTokenValue(struct Token* token);
struct Token* twoArgOpLewy(struct Token* op, struct Token* arg1, struct Token* arg2, struct Token* pomToken);
bool sprawdzCzyMozliweOperacjePrzyZwiekszeniu(int limitOperacji, int *liczbaOperacji);
struct Token* twoArgOp(struct Token* op, struct Token* arg1, struct Token* arg2);
struct Token*  oneArgOp(struct Token* op, struct Token* arg);

#endif // EVALUATE_H