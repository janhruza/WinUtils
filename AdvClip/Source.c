#include <Windows.h>
#include "resource.h"

#include "inc/WndHelper.h"
#include "inc/actions.h"

BOOL bInternalUpdate = FALSE; // Flag to prevent recursive clipboard updates
wchar_t szLastClipboardText[1024] = L"";

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	default:
		return FALSE;

		case WM_INITDIALOG:
			WHCreateAppMenu(hDlg);
			AddClipboardFormatListener(hDlg);

			break;

		case WM_CLOSE:
			EndDialog(hDlg, 0);
			return TRUE;

		case WM_RBUTTONDOWN:
		{
			POINT pt;
			GetCursorPos(&pt);
			WHShowContextMenu(hDlg, pt.x, pt.y);
			break;
		}

		case WM_COMMAND:
		{
			switch (wParam)
			{
				// cose the dialog window
				case IDC_BTN_CLOSE:
					EndDialog(hDlg, 0);
					break;

				// copies the selected content from clipboard history
				// to the clipboard
				case IDC_BTN_COPY:
					// copies the selected item's text to the clipboard
					bInternalUpdate = TRUE;
					HWND hListBox = GetDlgItem(hDlg, IDC_LB_HISTORY);
					int selIndex = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0);
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

				// clear the clipboard history
				case IDC_BTN_CLEAR:
                    if (MessageBox(hDlg, L"Are you sure you want to clear the clipboard history?", L"Confirm Clear", MB_ICONQUESTION | MB_YESNO) == IDYES)
                    {
						if (OpenClipboard(hDlg))
						{
							EmptyClipboard();
							CloseClipboard();
						}

						bInternalUpdate = TRUE;
                        HWND hListBox = GetDlgItem(hDlg, IDC_LB_HISTORY);
                        SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
						szLastClipboardText[0] = L'\0';
						bInternalUpdate = FALSE;
						break;
                    }

				default: break;
			}
		}

		case WM_CLIPBOARDUPDATE: {
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
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN_FORM), NULL, DlgProc);
	return 0;
}