#ifndef ZMIENNA_H
#define ZMIENNA_H
#include <conio.h>
#include <stdlib.h>
#include<stdio.h>

#define _CRT_SECURE_NO_WARNINGS

#define MAKSYMALNY_ROZMIAR_ZMIENNEJ 64

struct Zmienna {
	char* nazwa;
	int wartosc;
	bool jestPusta;
};

void strCopy(char dest[], char src[]);
bool jestLitera(char c);
bool jestCyfra(char c);
bool jestBialy(char c);
bool koniecWiersza(char c);
void wypiszZmienna(struct Zmienna zmienna);

#endif // ZMIENNA_H