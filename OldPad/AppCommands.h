#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <commdlg.h>

inline void AcNewWindow(void)
{
	HMODULE hModule = GetModuleHandle(NULL);
	if (!hModule)
	{
		return;
	}

	WCHAR wPath[MAX_PATH];
	GetModuleFileName(hModule, wPath, MAX_PATH);

	ShellExecute(NULL, L"open", wPath, NULL, NULL, SW_SHOW);
	return;
}

inline void AcChangeFont(HWND hEdit)
{
	if (!hEdit)
	{
		MessageBox(NULL, L"ERROR", L"AcChangeFont", MB_OK | MB_ICONERROR);
		return;
	}

	HFONT font = SendMessage(hEdit, WM_GETFONT, 0, 0);

	LOGFONTW lf = { 0 };
	CHOOSEFONTW cf = { 0 };
	ZeroMemory(&lf, sizeof(LOGFONTW));
	ZeroMemory(&cf, sizeof(CHOOSEFONTW));
	cf.lStructSize = sizeof(CHOOSEFONTW);
	cf.hwndOwner = hEdit;
	cf.lpLogFont = &lf;
	ChooseFont(&cf);

	HFONT hFont = CreateFontIndirect(&lf);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
	return;
}