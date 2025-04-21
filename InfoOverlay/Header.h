#pragma once
#include "resource.h"
#include <stdio.h>
#include <Windows.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// definitions
static HWND hCbxWindow;
static HWND hTxtOverlay;

static BOOL SetOverlayText(_In_ HWND hWnd, _In_ LPWSTR lpszText)
{
	if (!hWnd)
	{
		return FALSE;
	}

	if (IsWindowVisible(hWnd) == FALSE)
	{
		return FALSE;
	}

	// get client rect
	RECT rect;
	GetClientRect(hWnd, &rect);

	// draw text
	HDC hdc = GetDC(hWnd);
	DrawText(hdc, lpszText, lstrlen(lpszText), &rect, DT_RIGHT | DT_TOP);
	ReleaseDC(hWnd, hdc);

	return TRUE;
}

static BOOL UnsetOverlayText(_In_ HWND hWnd)
{
	if (!hWnd)
	{
		return FALSE;
	}

	if (IsWindowVisible(hWnd) == FALSE)
	{
		return FALSE;
	}

	// get client
	RECT rect;
	GetClientRect(hWnd, &rect);

	// remove text
	HDC hdc = GetDC(hWnd);
	FillRect(hdc, &rect, (HBRUSH)(COLOR_BACKGROUND + 1));
	ReleaseDC(hWnd, hdc);
	return TRUE;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	if (IsWindowVisible(hWnd) == FALSE)
	{
		// skip
		return TRUE;
	}

	WCHAR wndText[MAX_PATH];
	GetWindowText(hWnd, wndText, MAX_PATH);

	if (lstrlen(wndText) == 0)
	{
		swprintf_s(wndText, MAX_PATH, L"[No Text]\0");
	}

	const int size = 2 * MAX_PATH + 4;
	WCHAR wText[2 * MAX_PATH + 4];
	WCHAR wClass[MAX_PATH];

	RealGetWindowClass(hWnd, wClass, MAX_PATH);
	swprintf_s(wText, size, L"%s [%s]\0", wndText, wClass);

	int index = SendMessage(hCbxWindow, CB_ADDSTRING, NULL, (LPARAM)wText);
	SendMessage(hCbxWindow, CB_SETITEMDATA, index, (LPARAM)hWnd);
	return TRUE;
}

static void EnumAllWindows(void)
{
	if (!hCbxWindow) return;

	SendMessage(hCbxWindow, CB_RESETCONTENT, 0, 0);
	EnumWindows(EnumWindowsProc, NULL);
	SendMessage(hCbxWindow, CB_SETCURSEL, 0, 0);
	return;
}