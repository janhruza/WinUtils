#include "..\inc/WndHelper.h"
#include "..\inc\actions.h"
#include <Windows.h>

void CopyToClipboard(const wchar_t* content)
{
	if (OpenClipboard(NULL))
	{
		EmptyClipboard();
		size_t contentSize = (wcslen(content) + 1) * sizeof(wchar_t);
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, contentSize);
		if (hGlobal)
		{
			wchar_t* pGlobal = (wchar_t*)GlobalLock(hGlobal);
			if (pGlobal)
			{
				wcscpy_s(pGlobal, contentSize / sizeof(wchar_t), content);
				GlobalUnlock(hGlobal);
				SetClipboardData(CF_UNICODETEXT, hGlobal);
			}
			else
			{
				GlobalFree(hGlobal);
			}
		}
		CloseClipboard();
	}
}

void ACClearHistory()
{
	return;
}

void ACAddToHistory(HWND hListBox, const wchar_t* text)
{
	WHAppendListBoxItem(hListBox, text);
	return;
}