#include "zmienna.h"
#define _CRT_SECURE_NO_WARNINGS


void strCopy(char dest[], char src[]) {
	int i = 0;
	while (src[i] != '\0') {
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}
bool jestLitera(char c) {
	return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z';
}

bool jestCyfra(char c) {
	return c >= '0' && c <= '9';
}

bool jestBialy(char c) {
	return c == ' ' || c == '\n' || c == '\t';
}

bool koniecWiersza(char c) {
	return c == '\n' || c == '\0';
}

void wypiszZmienna(struct Zmienna zmienna) {
	if (zmienna.jestPusta)
		printf("%s Nul\n", zmienna.nazwa);
	else
		printf("%s %d\n", zmienna.nazwa, zmienna.wartosc);
}
