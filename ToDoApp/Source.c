#include <Windows.h>
#include "resource.h"
#include "inc/TaskItem.h"
#include "inc/Dialog/DlgNewTask.h"
#include "inc/TaskDb.h"

#define TASKS_DB_FILE	TEXT("tasks.bin")

TaskDB dbTasks;

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		dbTasks.nCount = 0;
		dbTasks.tasks = NULL;

		if (LoadTasksFromFile(TASKS_DB_FILE, &dbTasks))
		{
			for (int i = 0; i < dbTasks.nCount; i++)
			{
				PTaskItem item = dbTasks.tasks[i];
				int selItem = (int)SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_ADDSTRING, 0, (LPARAM)item->szTitle);
				SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_SETITEMDATA, selItem, (LPARAM)item);
			}
		}

		EnableWindow(GetDlgItem(hDlg, IDC_BTN_DELETE), FALSE);
		return TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN_NEW:
		{
			PTaskItem result = (PTaskItem)DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_NEWTASK), hDlg, DlgNewTaskProc);
			if (result != NULL && result != (PTaskItem)IDCANCEL)
			{
				if (AddTaskToDb(&dbTasks, result))
				{
					int selItem = (int)SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_ADDSTRING, 0, (LPARAM)result->szTitle);
					SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_SETITEMDATA, selItem, (LPARAM)result);
				}
				return TRUE;
			}
			return FALSE;
		}

		case IDC_BTN_DELETE:
		{
			int index = (int)SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_GETCURSEL, 0, 0);
			if (index != LB_ERR)
			{
				PTaskItem taskToDelete = (PTaskItem)SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_GETITEMDATA, index, 0);
				SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_DELETESTRING, index, 0);
				free(taskToDelete);
				for (int i = index; i < dbTasks.nCount - 1; i++) {
					dbTasks.tasks[i] = dbTasks.tasks[i + 1];
				}
				dbTasks.nCount--;
				if (dbTasks.nCount > 0) {
					dbTasks.tasks = (PTaskItem*)realloc(dbTasks.tasks, sizeof(PTaskItem) * dbTasks.nCount);
				}
				else {
					free(dbTasks.tasks);
					dbTasks.tasks = NULL;
				}

				EnableWindow(GetDlgItem(hDlg, IDC_BTN_DELETE), FALSE);
				return TRUE;
			}
			return FALSE;
		}

		case IDC_LIST_TASKS:
			if (HIWORD(wParam) == LBN_DBLCLK) {
				int index = (int)SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_GETCURSEL, 0, 0);
				if (index != LB_ERR) {
					PTaskItem task = (PTaskItem)SendDlgItemMessage(hDlg, IDC_LIST_TASKS, LB_GETITEMDATA, index, 0);
					MessageBox(hDlg, task->szDescription, task->szTitle, MB_OK | MB_ICONINFORMATION);
				}
			}
			else if (HIWORD(wParam) == LBN_SELCHANGE) {
				EnableWindow(GetDlgItem(hDlg, IDC_BTN_DELETE), TRUE);
			}
			return TRUE;

		case IDCANCEL:
		case IDC_BTN_CANCEL:
			PostMessage(hDlg, WM_CLOSE, 0, 0);
			return TRUE;
		}
		break;

	case WM_CLOSE:
		SaveTasksToFile(TASKS_DB_FILE, &dbTasks);
		FreeDb(&dbTasks);
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