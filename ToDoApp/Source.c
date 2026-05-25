#include <Windows.h>
#include "resource.h"
#include "inc/TaskItem.h"
#include "inc/Dialog/DlgNewTask.h"

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_BTN_NEW:
					PTaskItem result = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_NEWTASK), hDlg, DlgNewTaskProc);
					if (result != NULL)
					{
						SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_ADDSTRING, 0, (LPARAM)result->szTitle);
						SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_SETITEMDATA, SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_GETCOUNT, 0, 0) - 1, (LPARAM)result);
						return TRUE;
					}

					return FALSE;

				case IDC_BTN_CANCEL:
					EndDialog(hDlg, 0);
					return TRUE;

				case IDC_BTN_DELETE:
				{
					int index = SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_GETCURSEL, 0, 0);
					if (index != LB_ERR)
					{
						PTaskItem task = (PTaskItem)SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_GETITEMDATA, index, 0);
						free(task);
						SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_DELETESTRING, index, 0);
						
						int count = SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_GETCOUNT, 0, 0);
						if (count == 0)
						{
							EnableWindow(GetDlgItem(hDlg, IDC_BTN_DELETE), FALSE);
						}

						return TRUE;
					}
					return FALSE;
				}

				case IDC_LIST_TASKS:
					switch (HIWORD(wParam))
					{
						case LBN_DBLCLK:
						{
							int index = SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_GETCURSEL, 0, 0);
							if (index != LB_ERR)
							{
								PTaskItem task = (PTaskItem)SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_GETITEMDATA, index, 0);
								MessageBox(hDlg, task->szDescription, task->szTitle, MB_OK | MB_ICONINFORMATION);
								return TRUE;
							}

							return FALSE;
						}

						case LBN_SELCHANGE:
						{
							int index = SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_GETCURSEL, 0, 0);
							if (index == LB_ERR)
							{
								EnableWindow(GetDlgItem(hDlg, IDC_BTN_DELETE), FALSE);

							}
							else
							{
								EnableWindow(GetDlgItem(hDlg, IDC_BTN_DELETE), TRUE);
							}
						}
					}

				default: return FALSE;
			}

		case WM_CLOSE:
			EndDialog(hDlg, 0);
			return TRUE;
	}

	return FALSE;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINWINDOW), NULL, DialogProc);
	return EXIT_SUCCESS;
}