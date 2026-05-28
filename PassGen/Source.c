#include <Windows.h>

#include "res/resource.h"
#include "inc/PGCore.h"

#include <stdlib.h>
#include <time.h>
#include <CommCtrl.h>

INT_PTR CALLBACK DlgProc(HWND hDlg, int msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			// initializes the UI
			HWND hEdit = GetDlgItem(hDlg, IDC_EDIT_LENGTH);
			HWND hUpDown = GetDlgItem(hDlg, IDC_UPDOWN_LENGTH);
			SendMessage(hUpDown, UDM_SETBUDDY, (WPARAM)hEdit, 0);
			SendMessage(hUpDown, UDM_SETRANGE32, 6, 64);
			SetDlgItemInt(hDlg, IDC_EDIT_LENGTH, 16, FALSE);

			CheckDlgButton(hDlg, IDC_CHK_LOWERCASE, TRUE);
			return TRUE;
		}

		case WM_CLOSE:
			EndDialog(hDlg, IDCLOSE);
			return TRUE;

		case WM_SYSCOMMAND:
		{
			switch (LOWORD(wParam))
			{
				case SC_CLOSE:
					SendMessage(hDlg, WM_CLOSE, 0, 0);
					return TRUE;
			}
		}

		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			switch (wmId)
			{
				// the 'Cancel' button
				case IDCANCEL:
					EndDialog(hDlg, IDCANCEL);
					return TRUE;

				// the 'Generate' button
				case IDOK:
				{
					// get the password length
					HWND hOut = GetDlgItem(hDlg, IDC_EDIT_PASSWORD);
					HWND hEditLen = GetDlgItem(hDlg, IDC_EDIT_LENGTH);
					WCHAR text[10];
					GetWindowText(hEditLen, text, 10);
					int len = _wtoi(text);

					// get the char sets
					int cModes = CTYPE_NONE;

					// IsDlgButtonChecked vrací BST_CHECKED (1), pokud je checkbox zaškrtnutý
					if (IsDlgButtonChecked(hDlg, IDC_CHK_LOWERCASE) == BST_CHECKED) {
						cModes |= CTYPE_LOWER;
					}
					if (IsDlgButtonChecked(hDlg, IDC_CHK_UPPERCASE) == BST_CHECKED) {
						cModes |= CTYPE_UPPER;
					}
					if (IsDlgButtonChecked(hDlg, IDC_CHK_NUMBERS) == BST_CHECKED) {
						cModes |= CTYPE_NUM;
					}
					if (IsDlgButtonChecked(hDlg, IDC_CHK_SPECIAL) == BST_CHECKED) {
						cModes |= CTYPE_SPECIAL;
					}

					BOOL genRes = PgGeneratePassword(hOut, len, cModes);
					if (genRes == FALSE)
					{
						MessageBox(hDlg, TEXT("Unable to generate password."), TEXT("ERROR"), MB_OK | MB_ICONERROR);
					}

					return TRUE;
				}

				case IDC_EDIT_LENGTH:
				{
					// password length
					return TRUE;
				}

				case IDC_UPDOWN_LENGTH:
				{

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow)
{
	// initialize random generator
	srand(time(NULL));

	// show the dialog
	DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN_WINDOW), NULL, DlgProc);
	return 0;
}