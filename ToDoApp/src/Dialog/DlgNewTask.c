#include "..\..\inc\Dialog\DlgNewTask.h"
#include "..\..\resource.h"
#include "..\..\inc\TaskItem.h"

BOOL CreateTask(HWND hDlg, PTaskItem pTaskItem)
{
	if (!hDlg) return FALSE;

	pTaskItem = (PTaskItem)malloc(sizeof(TaskItem));

	return TRUE;
}

INT_PTR CALLBACK DlgNewTaskProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDCANCEL:
					EndDialog(hDlg, LOWORD(wParam));
					return (INT_PTR)TRUE;

				case IDC_BTN_ADD:
					PTaskItem pTask = CreateTaskFromDialog(hDlg);
					if (!pTask)
					{
						// task not created
						MessageBox(hDlg, TEXT("Failed to create task. Please try again."), TEXT("Error"), MB_ICONERROR | MB_OK);
						return FALSE;
					}

					// task created
					EndDialog(hDlg, pTask);
					return TRUE;

				default: return FALSE;
			}

		case WM_CLOSE:
			EndDialog(hDlg, NULL);
			return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}