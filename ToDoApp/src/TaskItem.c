#include <Windows.h>

#include "..\inc\TaskItem.h"
#include "..\resource.h"

PTaskItem CreateTaskFromDialog(HWND hDlg)
{
	if (!hDlg) return NULL;
	PTaskItem pTaskItem = (PTaskItem)malloc(sizeof(TaskItem));
	if (pTaskItem == NULL) return NULL;

	memset(pTaskItem, 0, sizeof(TaskItem));
	GetDlgItemText(hDlg, IDC_TXT_TITLE, pTaskItem->szTitle, sizeof(pTaskItem->szTitle));
	GetDlgItemText(hDlg, IDC_TXT_DESC, pTaskItem->szDescription, sizeof(pTaskItem->szDescription));

	if (lstrlen(pTaskItem->szTitle) == 0)
	{
		// task title cannot be null
		free(pTaskItem);
		return NULL;
	}

	if (lstrlen(pTaskItem->szDescription) == 0)
	{
		// description can be null, but is set manually to prevent inproper display in the listbox
		lstrcpy(pTaskItem->szDescription, TEXT("No description."));
	}

	return pTaskItem;
}

void FreeTask(PTaskItem pTaskItem)
{
	if (pTaskItem) free(pTaskItem);
}