#include "..\inc\WndHelper.h"

#include <Windows.h>

HMENU hCtxMenu;
HMENU hSystemMenu;

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

void WHCreateSystemMenu(HWND hDlg)
{
	hSystemMenu = GetSystemMenu(hDlg, FALSE);
	if (hSystemMenu)
	{
		AppendMenu(hSystemMenu, MF_SEPARATOR, 0, L"");
		AppendMenu(hSystemMenu, MF_STRING, ID_FORCE_EXIT, L"Force Exit\tCtrl+Q");
		ModifyMenu(hSystemMenu, SC_CLOSE, MF_BYCOMMAND, SC_CLOSE, L"Close Window\tAlt+F4");
	}
}