#pragma once
#include <Windows.h>

inline BOOL CsSendKeys(HWND hWnd, LPWSTR lpszKeys)
{
	if (!hWnd)
	{
		return FALSE;
	}

	// TODO missing implementation
	return TRUE;
}