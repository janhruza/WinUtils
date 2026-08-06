#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <commdlg.h>

LOGFONT g_lf = { 0 };

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
		return;
	}

	// Initialize g_lf from the edit control's current font the first time.
	if (g_lf.lfHeight == 0)
	{
		HFONT hFont = (HFONT)SendMessage(hEdit, WM_GETFONT, 0, 0);
		if (hFont)
		{
			GetObject(hFont, sizeof(LOGFONT), &g_lf);
		}
		else
		{
			// Fall back to the default GUI font if control has no explicit font.
			HFONT hDef = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			if (hDef)
			{
				GetObject(hDef, sizeof(LOGFONT), &g_lf);
			}
		}
	}

	CHOOSEFONT cf = { 0 };
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = hEdit;
	cf.lpLogFont = &g_lf;
	cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;

	if (ChooseFont(&cf))
	{
		HFONT hNewFont = CreateFontIndirect(&g_lf);

		// Store the created font handle on the window so we can delete it later.
		// If a previously created font exists, delete it.
		HANDLE hPrev = GetProp(hEdit, L"OldPad_Font");
		if (hPrev && hPrev != (HANDLE)hNewFont)
		{
			DeleteObject(hPrev);
		}

		SetProp(hEdit, L"OldPad_Font", (HANDLE)hNewFont);
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hNewFont, TRUE);
	}

	return;
}