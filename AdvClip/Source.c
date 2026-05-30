#include <Windows.h>
#include "resource.h"
#include <dwmapi.h>
#include <stdio.h>

#include "inc/WndHelper.h"
#include "inc/actions.h"

#pragma comment(lib, "dwmapi")
#pragma comment(lib, "uxtheme")

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

BOOL bInternalUpdate = FALSE; // Flag to prevent recursive clipboard updates
wchar_t szLastClipboardText[1024] = L"";

typedef enum {
	PreferredAppMode_Default,
	PreferredAppMode_AllowDark,
	PreferredAppMode_ForceDark,
	PreferredAppMode_ForceLight,
	PreferredAppMode_Max
} PreferredAppMode;

typedef PreferredAppMode(WINAPI* fnSetPreferredAppMode)(PreferredAppMode);
typedef void (WINAPI* fnFlushMenuThemes)(void);

void EnableDarkMenus(void)
{
	HMODULE hUxtheme = LoadLibraryExW(L"uxtheme.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hUxtheme)
	{
		fnSetPreferredAppMode SetPreferredAppMode =
			(fnSetPreferredAppMode)GetProcAddress(hUxtheme, (LPCSTR)MAKEINTRESOURCE(135));

		if (SetPreferredAppMode)
		{
			SetPreferredAppMode(PreferredAppMode_AllowDark);
		}

		fnFlushMenuThemes FlushMenuThemes =
			(fnFlushMenuThemes)GetProcAddress(hUxtheme, (LPCSTR)MAKEINTRESOURCE(136));

		if (FlushMenuThemes)
		{
			FlushMenuThemes();
		}

		FreeLibrary(hUxtheme);
	}
}

HBRUSH hBr;

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	default:
		return FALSE;

	case WM_CTLCOLORSTATIC:
		SetBkMode((HDC)wParam, TRANSPARENT);
		SetTextColor((HDC)wParam, RGB(0xFF, 0xFF, 0xFF));
		return (INT_PTR)hBr;

	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
		return (INT_PTR)hBr;

	case WM_CTLCOLORLISTBOX:
		SetBkColor((HDC)wParam, RGB(30, 30, 30));
		return (INT_PTR)hBr;

	case WM_INITDIALOG:
	{
		BOOL darkMode = TRUE;
		HWND hListBox;
		LONG_PTR style;

		WHCreateSystemMenu(hDlg);
		WHCreateAppMenu(hDlg);
		AddClipboardFormatListener(hDlg);

		hBr = CreateSolidBrush(RGB(30, 30, 30));
		DwmSetWindowAttribute(hDlg, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(BOOL));
		COLORREF captionBg = RGB(30, 30, 30);
		DwmSetWindowAttribute(hDlg, DWMWA_CAPTION_COLOR, &captionBg, sizeof(COLORREF));

		hListBox = GetDlgItem(hDlg, IDC_LB_HISTORY);
		style = GetWindowLongPtr(hListBox, GWL_STYLE);

		/* Odstraníme WS_BORDER bitovou maskou */
		SetWindowLongPtr(hListBox, GWL_STYLE, style & ~WS_BORDER);
		/* Vynutíme překreslení okrajů, aby změna byla okamžitě vidět */
		SetWindowPos(hListBox, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		EnableDarkMenus();

		break;
	}

	case WM_CLOSE:
		CloseWindow(hDlg);
		return TRUE;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;

		/* Zkontrolujeme, jestli se kreslí tlačítko */
		if (pdis->CtlType == ODT_BUTTON)
		{
			COLORREF bgColor = (pdis->itemState & ODS_SELECTED) ? RGB(70, 70, 70) : RGB(45, 45, 45);
			COLORREF textColor = RGB(255, 255, 255);
			COLORREF borderColor = RGB(20, 20, 20);

			HBRUSH hBrush = CreateSolidBrush(bgColor);
			HPEN hPen = CreatePen(PS_SOLID, 1, borderColor);

			HBRUSH hOldBrush = (HBRUSH)SelectObject(pdis->hDC, hBrush);
			HPEN hOldPen = (HPEN)SelectObject(pdis->hDC, hPen);

			RoundRect(pdis->hDC, pdis->rcItem.left, pdis->rcItem.top, pdis->rcItem.right, pdis->rcItem.bottom, 8, 8);

			SelectObject(pdis->hDC, hOldBrush);
			SelectObject(pdis->hDC, hOldPen);
			DeleteObject(hBrush);
			DeleteObject(hPen);

			wchar_t btnText[256];
			GetWindowTextW(pdis->hwndItem, btnText, 256);

			SetBkMode(pdis->hDC, TRANSPARENT);
			SetTextColor(pdis->hDC, textColor);
			DrawTextW(pdis->hDC, btnText, -1, &pdis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			return TRUE;
		}
		else if (pdis->CtlType == ODT_LISTBOX)
		{
			COLORREF bgColor;
			COLORREF textColor;

			if (pdis->itemState & ODS_SELECTED)
			{
				bgColor = RGB(0, 120, 215);
				textColor = RGB(255, 255, 255);
			}
			else
			{
				bgColor = RGB(30, 30, 30);
				textColor = RGB(200, 200, 200);
			}

			HBRUSH hBrush = CreateSolidBrush(bgColor);
			FillRect(pdis->hDC, &pdis->rcItem, hBrush);
			DeleteObject(hBrush);

			wchar_t itemText[256];
			SendMessageW(pdis->hwndItem, LB_GETTEXT, pdis->itemID, (LPARAM)itemText);

			SetBkMode(pdis->hDC, TRANSPARENT);
			SetTextColor(pdis->hDC, textColor);

			RECT textRect = pdis->rcItem;
			textRect.left += 5;

			DrawTextW(pdis->hDC, itemText, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

			return TRUE;
		}
		break;
	}

	case WM_RBUTTONDOWN:
	{
		POINT pt;
		GetCursorPos(&pt);
		WHShowContextMenu(hDlg, pt.x, pt.y);
		break;
	}

	case WM_SYSCOMMAND:

		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case SC_CLOSE:
			CloseWindow(hDlg);
			return TRUE;

		case ID_FORCE_EXIT:
			DestroyWindow(hDlg);
			break;

		default: return FALSE;
		}

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case IDC_BTN_CLOSE:
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;

		case IDC_BTN_COPY:
		{
			HWND hListBox = GetDlgItem(hDlg, IDC_LB_HISTORY);
			int selIndex = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0);

			bInternalUpdate = TRUE;

			if (selIndex != LB_ERR)
			{
				wchar_t buffer[1024];
				SendMessage(hListBox, LB_GETTEXT, selIndex, (LPARAM)buffer);
				if (OpenClipboard(hDlg))
				{
					EmptyClipboard();
					HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, (wcslen(buffer) + 1) * sizeof(wchar_t));
					if (hGlob)
					{
						wchar_t* pGlob = (wchar_t*)GlobalLock(hGlob);
						if (pGlob)
						{
							wcscpy_s(pGlob, wcslen(buffer) + 1, buffer);
							GlobalUnlock(hGlob);
							SetClipboardData(CF_UNICODETEXT, hGlob);
						}
						else
						{
							GlobalFree(hGlob);
						}
					}
					CloseClipboard();
				}
			}
			bInternalUpdate = FALSE;
			break;
		}

		case ID_CLEAR_ALL:
		case IDC_BTN_CLEAR:
		{
			if (MessageBox(hDlg, L"Are you sure you want to clear the clipboard history?", L"Confirm Clear", MB_ICONQUESTION | MB_YESNO) == IDYES)
			{
				HWND hListBox;

				if (OpenClipboard(hDlg))
				{
					EmptyClipboard();
					CloseClipboard();
				}

				bInternalUpdate = TRUE;
				hListBox = GetDlgItem(hDlg, IDC_LB_HISTORY);
				SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
				szLastClipboardText[0] = L'\0';
				bInternalUpdate = FALSE;
			}
			break;
		}

		case ID_REFRESH:
			bInternalUpdate = TRUE;
			SendMessage(hDlg, WM_CLIPBOARDUPDATE, NULL, NULL);
			bInternalUpdate = FALSE;
			break;

		case ID_CLOSE_WINDOW:
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			return TRUE;

		default:
			break;
		}
		break;
	}

	case WM_LBUTTONDOWN:
		//SetFocus(hDlg);
		return TRUE;

	case WM_CLIPBOARDUPDATE:
	{
		if (bInternalUpdate == TRUE)
			break;

		if (OpenClipboard(hDlg))
		{
			HANDLE hData = GetClipboardData(CF_UNICODETEXT);
			if (hData)
			{
				LPCWSTR lpszData = (LPCWSTR)GlobalLock(hData);
				if (lpszData)
				{
					if (wcscmp(lpszData, szLastClipboardText) != 0)
					{
						HWND hListBox = GetDlgItem(hDlg, IDC_LB_HISTORY);
						SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)lpszData);
						wcsncpy_s(szLastClipboardText, 1024, lpszData, 1023);
						szLastClipboardText[1023] = L'\0';
					}
					GlobalUnlock(hData);
				}
			}
			CloseClipboard();
		}
		break;
	}
	}

	return TRUE;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN_FORM), NULL, DlgProc);
	return 0;
}