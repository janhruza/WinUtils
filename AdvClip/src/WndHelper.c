#include "..\inc\WndHelper.h"

#include <Windows.h>

HMENU hCtxMenu;

void WHCreateAppMenu(HWND hDlg)
{
	hCtxMenu = CreatePopupMenu();
	if (hCtxMenu)
	{
		AppendMenu(hCtxMenu, MF_STRING, ID_REFRESH, L"Refresh\tF5");
		//AppendMenu(hCtxMenu, MF_SEPARATOR, 0x00, L"");
		AppendMenu(hCtxMenu, MF_STRING, ID_CLEAR_ALL, L"Clear History\tDel");
		AppendMenu(hCtxMenu, MF_SEPARATOR, 0x00, L"");
		AppendMenu(hCtxMenu, MF_STRING, ID_CLOSE_WINDOW, L"Close\tAlt+F4");
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