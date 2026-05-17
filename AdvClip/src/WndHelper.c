#include "..\inc\WndHelper.h"

#include <Windows.h>

HMENU hCtxMenu;

void WHCreateAppMenu(HWND hDlg)
{
	hCtxMenu = CreatePopupMenu();
	if (hCtxMenu)
	{
		AppendMenu(hCtxMenu, MF_STRING, 1, L"Refresh\tF5");
		//AppendMenu(hCtxMenu, MF_SEPARATOR, 0x00, L"");
		AppendMenu(hCtxMenu, MF_STRING, 2, L"Clear History\tDel");
	}
}

void WHShowContextMenu(HWND hDlg, int x, int y)
{
	if (hCtxMenu)
	{
		TrackPopupMenu(hCtxMenu, TPM_LEFTALIGN | TPM_TOPALIGN, x, y, 0, hDlg, NULL);
	}
}

void WHAppendListBoxItem(HWND hListBox, LPCWSTR text)
{
	SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)text);
}