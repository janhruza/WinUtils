#pragma once
#ifndef PGCORE_H
#define PGCORE_H

#include <Windows.h>

#define PASS_MAXLEN 128

typedef enum {
	CTYPE_NONE = 0,
	CTYPE_LOWER = 1 << 0,
	CTYPE_UPPER = 1 << 1,
	CTYPE_NUM = 1 << 2,
	CTYPE_SPECIAL = 1 << 3
} CharTypeFlags;

extern LPWSTR lpszLowercase;
extern LPWSTR lpszUppercase;
extern LPWSTR lpszDigits;
extern LPWSTR lpszSpecials;

int PgGetRandomIndex(LPWSTR lpChars);

BOOL PgGeneratePassword(HWND hOut, int len, int charTypes);

#endif
