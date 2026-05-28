#include "..\inc\PGCore.h"

#include <stdlib.h>
#include <time.h>

LPWSTR lpszLowercase = L"abcdefghijklmnopqrstuvwxyz";
LPWSTR lpszUppercase = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
LPWSTR lpszDigits = L"0123456789";
LPWSTR lpszSpecials = L".,:?!;-+/*\"\'/()[]{}<>|&@#";

int PgGetRandomIndex(LPWSTR lpChars)
{
	if (lpChars == NULL) return -1;
	int len = lstrlen(lpChars);
	if (len == 0) return -1;
	return rand() % len;
}

BOOL PgGeneratePassword(HWND hOut, int len, int charTypes)
{
	// check input data
	if (hOut == NULL) return FALSE;
	if (len > PASS_MAXLEN - 1) len = PASS_MAXLEN - 1;

	// initialize the buffer
	WCHAR arr[PASS_MAXLEN];

	// get the available password symbols
	WCHAR szAllowedChars[256] = L"";
	if (charTypes & CTYPE_LOWER) {
		wcscat_s(szAllowedChars, 256, lpszLowercase);
	}
	if (charTypes & CTYPE_UPPER) {
		wcscat_s(szAllowedChars, 256, lpszUppercase);
	}
	if (charTypes & CTYPE_NUM) {
		wcscat_s(szAllowedChars, 256, lpszDigits);
	}
	if (charTypes & CTYPE_SPECIAL) {
		wcscat_s(szAllowedChars, 256, lpszSpecials);
	}

	// fill the password
	size_t totalAllowed = wcslen(szAllowedChars);
	for (int i = 0; i < len; i++)
	{
		int idx = PgGetRandomIndex(szAllowedChars);
		if (idx == -1)
		{
			// critical, array empty
			return FALSE;
		}

		arr[i] = szAllowedChars[idx];
	}

	// null-terminate the output
	arr[len] = '\0';

	// print the password
	SetWindowText(hOut, arr);
	return TRUE;
}