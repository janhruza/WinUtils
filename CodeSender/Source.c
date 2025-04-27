#pragma once
#include "CSCore.h"

COLORREF hBg;
HBRUSH hbrBackground;
TargetWindow hwTarget;
HWND hDialog;
HMENU hPopupMenu;

// dialog controls
HWND hCbxWindows;
HWND hTxtInput;

// function definitions
inline BOOL RefreshWindowList(void);
inline void SendKeys(TargetWindow*);

inline HMENU CreateAppMenu()
{
	HMENU hMenu = CreatePopupMenu();

	AppendMenu(hMenu, MF_STRING, ID_ITEM_REFRESH, L"Refresh\tF5");
	AppendMenu(hMenu, MF_SEPARATOR, 0, L"Refresh\tF5");
	AppendMenu(hMenu, MF_STRING, ID_ITEM_EXIT, L"Close\tAlt+F4");

	return hMenu;
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:

		// get dialog window
		hDialog = hDlg;
		hPopupMenu = CreateAppMenu();

		// get colors
		hBg = RGB(255, 255, 255);
		hbrBackground = CreateSolidBrush(hBg);

		// get controls
		hCbxWindows = GetDlgItem(hDlg, IDC_CBX_WINDOW);
		hTxtInput = GetDlgItem(hDlg, IDC_TXT_CODE);

		// prepare UI
		RefreshWindowList();

		return TRUE;

	case WM_CTLCOLORDLG:
		return hbrBackground;

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORBTN:
		HDC hdc = GetDC(hDlg);
		SetBkMode(hdc, hBg);
		return hdc;

	case WM_COMMAND:
	{
		switch (wParam)
		{
			// cancel button
			case ID_ITEM_EXIT:
			case IDC_BTN_CANCEL:
				SendMessage(hDlg, WM_CLOSE, 0, 0);
				break;

			// send keys button
			case IDC_BTN_SEND:
			{
				// check for window
				int idx = SendMessage(hCbxWindows, CB_GETCURSEL, 0, 0);
				if (idx == CB_ERR)
				{
					// no item selected
					break;
				}

				// get window
				HWND hWnd = (HWND)SendMessage(hCbxWindows, CB_GETITEMDATA, idx, 0);
				if (hWnd == CB_ERR)
				{
					// window not found
					break;
				}

				// get text (input data)
				WCHAR pData[MAX_PATH];
				GetWindowText(hTxtInput, pData, MAX_PATH);

				// fill structure data
				hwTarget.hWnd = hWnd;
				hwTarget.keys = pData;

				// send keys
				SendKeys(&hwTarget);
			}
			break;

			// refresh windows list
			case ID_ITEM_REFRESH:
			case IDC_BTN_REFRESH:
				RefreshWindowList();
				break;
		}

		// WM_COMMAND handled
		return TRUE;
	}

	case WM_CLOSE:
		EndDialog(hDlg, IDCLOSE);
		return TRUE;

	case WM_RBUTTONDOWN:
	{
		// show context menu
		if (!hPopupMenu) break;

		POINT pt;
		GetCursorPos(&pt);
		TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, hDialog, NULL);
	}
	break;

	case WM_KEYDOWN:
	{
		int key = (int)wParam;
		switch (key)
		{
		case VK_F5:
			RefreshWindowList();
			break;

		break;
		}
	}
	break;

	default:
		return FALSE;
	}
	return FALSE;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	if (IsWindowVisible(hWnd) == FALSE)
	{
		return TRUE;
	}

	// get window details
	WCHAR title[MAX_PATH];
	GetWindowText(hWnd, title, MAX_PATH);

	// check window text length
	if (lstrlen(title) == 0)
	{
		// no window title
		// excluded from processing
		return TRUE;
	}

	// add menu item and its details
	// data is the window handle
	int idx = SendMessage(hCbxWindows, CB_ADDSTRING, 0, (LPARAM)title);
	SendMessage(hCbxWindows, CB_SETITEMDATA, idx, hWnd);
	return TRUE;
}

inline BOOL RefreshWindowList(void)
{
	SendMessage(hCbxWindows, CB_RESETCONTENT, 0, 0);
	BOOL value = EnumWindows(EnumWindowsProc, 0);
	int count = SendMessage(hCbxWindows, CB_GETCOUNT, 0, 0);
	if (count != CB_ERR && count > 0)
	{
		SendMessage(hCbxWindows, CB_SETCURSEL, 0, 0);
	}
	return value;
}

inline void SendKeys(TargetWindow* window)
{
	WCHAR buf[MAX_PATH];
	BOOL result = CsSendKeys(window->hWnd, window->keys);

	if (result == TRUE)
	{
		MessageBox(hDialog, L"Keys were sent!", L"SendKeys", MB_OK | MB_ICONINFORMATION);
	}

	else
	{
		MessageBox(hDialog, L"Unable to send keys to a selected window.", L"SendKeys", MB_OK | MB_ICONERROR);
	}

	return;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ INT nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINFORM), NULL, DlgProc);
	return 0;
}