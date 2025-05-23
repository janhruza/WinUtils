#pragma once
#include "Header.h"

static HBRUSH hBrush;

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		COLORREF color = RGB(0xFF, 0xFF, 0xFF);
		hBrush = CreateSolidBrush(color);
		DwmSetWindowAttribute(hDlg, DWMWA_CAPTION_COLOR, &color, sizeof(COLORREF));
		hCbxWindow = GetDlgItem(hDlg, IDC_CBX_WINDOW);
		hTxtOverlay = GetDlgItem(hDlg, IDC_TXT_OVERLAY);
		EnumAllWindows();
		return TRUE;
	}

		case WM_COMMAND:
		{
			switch (wParam)
			{
				// close dialog
				case IDC_BTN_CANCEL:
					EndDialog(hDlg, IDCANCEL);
					return TRUE;

				// refresh windows
				case IDC_BTN_REFRESH:
					EnumAllWindows();
					return TRUE;

				// apply text
				case IDC_BTN_OK:
					int index = SendMessage(hCbxWindow, CB_GETCURSEL, 0, 0);
					HWND hWnd = SendMessage(hCbxWindow, CB_GETITEMDATA, index, 0);

					// check if item has an associated window
					if (hWnd == CB_ERR) return TRUE;

					// get overlay text
					WCHAR overlayText[512];
					GetWindowText(hTxtOverlay, overlayText, 512);

					if (lstrlen(overlayText) == 0)
					{
						// remove any overlay
						UnsetOverlayText(hWnd);
						return TRUE;
					}

					// apply overlay text
					SetOverlayText(hWnd, overlayText);
					return TRUE;
			}

			return FALSE;
		}

		case WM_CTLCOLORDLG:
			return (INT_PTR)hBrush;

		case WM_CTLCOLORBTN:
		case WM_CTLCOLORSTATIC:
			HDC hdc = GetDC(hDlg);
			SetBkColor(hdc, TRANSPARENT);
			return hBrush;

		case WM_CLOSE:
			DeleteObject(hBrush);
			EndDialog(hDlg, IDCLOSE);
			return TRUE;
		}

	return FALSE;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCommandLine, int nCmdShow)
{
	// main entry point
	hCurrentInstance = hInstance;
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN_DIALOG), NULL, DialogProc);
	return 0;
}