#pragma once
#include "resource.h"
#include <stdio.h>
#include <Windows.h>
#include <CommCtrl.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include "List.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// definitions
static HWND hCbxWindow;
static HWND hTxtOverlay;
static HINSTANCE hCurrentInstance;

static BOOL SetOverlayText(_In_ HWND hWnd, _In_ LPWSTR lpszText)
{
	// check for storage space
	int idx = ListGetFreeIndex();
	if (idx == LIST_INVALID)
	{
		// can't store window
		// all save slots are full
		return FALSE;
	}

	WCHAR wClassName[MAX_CLASS_NAME];
	GetClassName(hWnd, wClassName, MAX_CLASS_NAME);

	RECT rect;
	GetClientRect(hWnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// create overlay window
	HWND hOverlay = CreateWindowEx(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT, // Extended window styles
		WC_STATIC,						// Window class
		lpszText,						// Window name
		WS_VISIBLE | WS_CHILD,			// Window style (no borders)
		rect.left,						// X position
		rect.top,						// Y position
		width,							// Width
		height,							// Height
		hWnd,							// Parent window
		NULL,							// Menu
		hCurrentInstance,				// Instance handle
		NULL							// Additional application data
	);

	// save window and the overlay
	return ListSetItem(idx, hWnd, hOverlay);
}

static BOOL UnsetOverlayText(_In_ HWND hWnd)
{
	// retrieve index for window and its overlay window
	int idx = ListGetIndex(hWnd);
	if (idx == LIST_INVALID)
	{
		// can't unset text
		// window not found
		return FALSE;
	}

	// destroy overlay window
	HWND hOverlay = ListGetOverlay(idx);
	if (hOverlay == LIST_NULL) return FALSE;

	// destroy overlay window
	DestroyWindow(hOverlay);

	// free handles from lists
	return ListSetItem(idx, NULL, NULL);
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