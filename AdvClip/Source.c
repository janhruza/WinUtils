#include <Windows.h>
#include "resource.h"

#include "inc/WndHelper.h"

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
			WHCreateAppMenu(hDlg);
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
					break;
			}
		}
		break;
	}
	return FALSE;
}

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN_FORM), NULL, DlgProc);
	return 0;
}